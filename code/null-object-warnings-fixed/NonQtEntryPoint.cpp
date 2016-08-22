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

#include "NonQtEntryPoint.h"

#include "NonQtMeter.h"

#include <QTimer>

NonQtEntryPoint::NonQtEntryPoint()
{
    resetListImpl();
}

NonQtEntryPoint::~NonQtEntryPoint() = default;

void NonQtEntryPoint::addResetObserver(std::function<void()> observer)
{
    mResetObservers.push_back(observer);
}

void NonQtEntryPoint::addRemoveObserver(std::function<void(int)> observer)
{
    mRemoveObservers.push_back(observer);
}

std::vector<std::unique_ptr<NonQtMeter>> const&
NonQtEntryPoint::getMeters() const
{
    return mMeters;
}

void NonQtEntryPoint::resetList()
{
    mTimers.clear();
    mMeters.clear();
    resetListImpl();
    for (auto observer : mResetObservers) {
        observer();
    }
}

void NonQtEntryPoint::removeMeter(int index)
{
    {
    auto pos = mTimers.begin() + index;
    mTimers.erase(pos);
    }
    {
    auto pos = mMeters.begin() + index;
    mMeters.erase(pos);
    }
    for (auto observer : mRemoveObservers) {
        observer(index);
    }
}

void NonQtEntryPoint::resetListImpl()
{
    // Create some meter objects and use timers to
    // randomly change their value over time.

    static const int minValue = 0;
    static const int maxValue = 250;

    for (auto i : {17, 200, 42, 150, 100})
    {
        mMeters.emplace_back(new NonQtMeter);
        auto& modelMeter = mMeters.back();
        modelMeter->setValue(i);

        auto unsafePtr = modelMeter.get();

        mTimers.emplace_back(new QTimer);
        auto changer = mTimers.back().get();

        int interval = (rand() % 1000) + 1000;
        changer->setInterval(interval);
        QObject::connect(changer, &QTimer::timeout, [unsafePtr]{
            int valueChange = (rand() % 20) - 10;
            int newValue = unsafePtr->getValue() + valueChange;
            newValue = std::max(minValue, std::min(newValue, maxValue));
            unsafePtr->setValue(newValue);
        });
        changer->start();
    }
}
