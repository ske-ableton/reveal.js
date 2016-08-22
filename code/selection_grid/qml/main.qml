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

import QtQuick 2.4
import QtQml.Models 2.2

import MyLib 1.0

Item {
    width: 1200
    height: 600
    objectName: "Root"

    MyModel {
        id: myGridModel
        objectName: "myGridModel"
    }

    SelectionAggregator {
        id: activeGridItemSelections
        model: myGridModel
        objectName: "activeGridItems"
        aggregationType: SelectionAggregator.SelectionAggregation
    }

    SelectionProxyModel {
        id: theSelectedGridItems
        sourceModel: myGridModel
        selectionModel: activeGridItemSelections
        objectName: "selectedGridItems"
    }

    SelectionAggregator {
        id: activeInListView
        model: myGridModel
        objectName: "activeInListView"
        aggregationType: SelectionAggregator.CurrentAggregation
    }

    GridItems {
        x: 10
        y: 10
        width: 600
        height: 300

        myModel: myGridModel
        activeGridItemSelections: activeGridItemSelections
        activeGridItems: activeInListView
    }

    SelectionView {
        y: 320
        x: 20
        width: 150
        height: 200

        selectedGridItems: theSelectedGridItems
        activeInListView: activeInListView
    }
}
