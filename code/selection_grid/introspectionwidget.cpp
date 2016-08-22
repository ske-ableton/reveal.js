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

#include "introspectionwidget.h"

#include <QAbstractItemModel>
#include <QItemSelectionModel>
#include <QQuickView>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTreeView>
#include <QQuickItem>
#include <QLabel>
#include <QComboBox>
#include <QDebug>
#include <QTimer>

#include "utility.h"

std::tuple<QList<QAbstractItemModel*>, QList<QItemSelectionModel*> > gatherObjects(QObject* rootObject)
{
    std::tuple<QList<QAbstractItemModel*>, QList<QItemSelectionModel*> > result;

    std::get<0>(result) = rootObject->findChildren<QAbstractItemModel*>();
    std::get<1>(result) = rootObject->findChildren<QItemSelectionModel*>();
    auto flickables = rootObject->findChildren<QObject*>();

    for (auto item: flickables)
    {
        if (item->inherits("QQuickItemView")) {
            auto lp = item->property("data").value<QQmlListProperty<QObject>>();

            for (int i = 0; i < lp.count(&lp); ++i)
            {
                if (!lp.at(&lp, i)->inherits("QQuickItem")) {
                    continue;
                }
                auto delegs = lp.at(&lp, i)->property("data").value<QQmlListProperty<QObject>>();

                auto numDelegs = delegs.count(&delegs);
                for (int j = 0; j < numDelegs; ++j)
                {
                    auto nextResult = gatherObjects(delegs.at(&delegs, j));
                    std::get<0>(result) += std::get<0>(nextResult);
                    std::get<1>(result) += std::get<1>(nextResult);
                }
            }
        }
    }
    return result;
}

static int sNum = 0;

IntrospectionWidget::IntrospectionWidget(QQuickView* quickView, QWidget* parent)
    : QWidget(parent), mQuickView(quickView)
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setMargin(0);
    mSplitter = new QSplitter;
    layout->addWidget(mSplitter);

    auto displayWidget = new QWidget;
    mLayout = new QVBoxLayout(displayWidget);
    mLayout->setSpacing(0);
    mLayout->setMargin(0);
    mGridModel = new QTreeView;
    mGridModel->setSelectionMode(QTreeView::ExtendedSelection);
    mLayout->addWidget(mGridModel);

    mSplitter->addWidget(displayWidget);

    QTimer::singleShot(500, this, &IntrospectionWidget::initialize);

    resize(900, 700);
}

QComboBox* getSelmCombo(QList<QItemSelectionModel*>& selms, QAbstractItemModel* aim, QTreeView* tv)
{
    QComboBox* combo = nullptr;

    QList<QItemSelectionModel*> mySelms;
    for (auto sel: selms)
    {
        if (sel->model() == aim)
        {
            mySelms.push_back(sel);
        }
    }
    for (auto i : mySelms)
    {
        selms.removeAll(i);
    }
    if (!mySelms.isEmpty())
    {
        combo = new QComboBox;
        combo->addItem("None");
        for (auto sel: mySelms) {
            QString name = sel->objectName();
            if (name.isEmpty())
            {
                name = "QISM" + QString::number(sNum++);
            }
            combo->addItem(name, QVariant::fromValue(sel));
        }
        QObject::connect(combo, SELECT<int>::OVERLOAD_OF(&QComboBox::activated), combo,
            [combo, tv] (int actived) {
                auto var = combo->itemData(actived);
                if (var.isValid()) {
                    tv->setSelectionModel(combo->itemData(actived).value<QItemSelectionModel*>());
                } else {
                    tv->setSelectionModel(new QItemSelectionModel(tv->model(), tv));
                }
                tv->viewport()->update();
        });
    }

    return combo;
}
QWidget* createAimWidget(QAbstractItemModel* aim, QList<QItemSelectionModel*>& selms, bool addLabel = true)
{
    auto w = new QWidget;
    auto l = new QVBoxLayout(w);
    l->setSpacing(0);
    l->setMargin(0);
    auto tv = new QTreeView;
    tv->setSelectionMode(QTreeView::ExtendedSelection);
    if (addLabel)
        l->addWidget(new QLabel(aim->objectName()));
    tv->setModel(aim);
    tv->expandAll();
    l->addWidget(tv);
    auto combo = getSelmCombo(selms, aim, tv);
    if (combo) {
        l->addWidget(combo);
    }
    return w;
}

void IntrospectionWidget::initialize()
{
    auto objects = gatherObjects(mQuickView->rootObject());
    auto aims = std::get<0>(objects);
    auto selms = std::get<1>(objects);

    auto gridItems = mQuickView->rootObject()->findChild<QQuickItem*>("theGridItems");

    auto gridItemsObjects = gatherObjects(gridItems);
    auto gridItemsAims = std::get<0>(gridItemsObjects);
    auto gridItemsSelms = std::get<1>(gridItemsObjects);

    for (auto i : gridItemsAims)
    {
        aims.removeAll(i);
    }
    for (auto i : gridItemsSelms)
    {
        if (i->objectName() == "crosshair")
            continue;
        selms.removeAll(i);
    }

    for (auto aim: aims) {
        if (aim->objectName() == "myGridModel") {
            mGridModel->setModel(aim);
            mGridModel->expandAll();
            mLayout->insertWidget(0, new QLabel(aim->objectName()));
            if (auto combo = getSelmCombo(selms, aim, mGridModel))
            {
                mLayout->addWidget(combo);
            }
            break;
        }
    }
    aims.removeAll(mGridModel->model());
}
