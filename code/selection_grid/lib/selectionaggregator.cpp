/* Copyright (c) 2016 Ableton AG, Berlin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "selectionaggregator.h"

SelectionAggregator::SelectionAggregator(QObject* parent)
    : QItemSelectionModel(nullptr, parent), mType(UndefinedAggregation)
{
    connect(this, &QItemSelectionModel::modelChanged, this, [this]{
        if (model() && mType != UndefinedAggregation) {
            initSatellites();
            handleSelectionChange();
        }
    });

    connect(this, &QItemSelectionModel::selectionChanged, this, [this] {
        if (mType == UndefinedAggregation) {
            return;
        }
        foreach(auto selModel, mSatellites) {
            if (!selModel) {
                removeSatellite(selModel);
                continue;
            }

            auto idx = mSatellites.indexOf(selModel);
            Q_ASSERT(idx < (int)mSatelliteMappers.size());
            auto mapper = mSatelliteMappers[idx].get();

            if (mType == CurrentAggregation) {
                // auto toAdd = mapper->mapLeftToRight(selModel->currentIndex());
                // sel.select(toAdd, toAdd);
            } else {
                mAvoidRecursion = true;
                auto sel = mapper->mapSelectionLeftToRight(this->selection());
                if (sel != selModel->selection()) {
                    selModel->clearSelection();
                    selModel->select(sel, QItemSelectionModel::ClearAndSelect);
                }
                mAvoidRecursion = false;
            }
        }
    });
}

void SelectionAggregator::setAggregationType(AggregationType type)
{
    if (mType != type) {
        mType = type;
        Q_EMIT aggregationTypeChanged();
        if (this->model()) {
            initSatellites();
            handleSelectionChange();
        }
    }
}

SelectionAggregator::AggregationType SelectionAggregator::aggregationType() const
{
    return mType;
}

void SelectionAggregator::initSatellites()
{
    mSatelliteMappers.clear();
    foreach(auto selModel, mSatellites) {
        if (!selModel) {
            removeSatellite(selModel);
            continue;
        }
        auto mapper = std::unique_ptr<KModelIndexProxyMapper>(
            new KModelIndexProxyMapper(this->model(), selModel->model()));
        mSatelliteMappers.emplace_back(std::move(mapper));

        if (mType == CurrentAggregation) {
            connect(selModel, &QItemSelectionModel::currentChanged, this,
                &SelectionAggregator::handleSelectionChange);
        } else {
            connect(selModel, &QItemSelectionModel::selectionChanged, this,
                &SelectionAggregator::handleSelectionChange);
        }

        connect(selModel, &QItemSelectionModel::destroyed, this,
            &SelectionAggregator::handleSelectionChange);

        connect(selModel, &QItemSelectionModel::modelChanged, this, [this]{
            if (model() && mType != UndefinedAggregation) {
                initSatellites();
                handleSelectionChange();
            }
        });
    }
}

void SelectionAggregator::addSatellite(QItemSelectionModel* satellite)
{
    mSatellites.push_back(satellite);
    if (mType != UndefinedAggregation) {
        initSatellites();
    }
}

void SelectionAggregator::removeSatellite(QItemSelectionModel* satellite)
{
    auto idx = mSatellites.indexOf(satellite);
    mSatellites.removeAt(idx);
    if (mType != UndefinedAggregation) {
        initSatellites();
    }
}

void SelectionAggregator::handleSelectionChange()
{
    if (mAvoidRecursion)
        return;
    QItemSelection sel;
    foreach(auto selModel, mSatellites) {
        if (!selModel) {
            removeSatellite(selModel);
            continue;
        }
        auto idx = mSatellites.indexOf(selModel);
        Q_ASSERT(idx < (int)mSatelliteMappers.size());
        auto mapper = mSatelliteMappers[idx].get();

        if (mType == CurrentAggregation) {
            auto toAdd = mapper->mapRightToLeft(selModel->currentIndex());
            sel.select(toAdd, toAdd);
        } else {
            auto toAdd = mapper->mapSelectionRightToLeft(selModel->selection());
            sel.merge(toAdd, QItemSelectionModel::Select);
        }
    }
    this->clearSelection();
    this->select(sel, QItemSelectionModel::Select);
}
