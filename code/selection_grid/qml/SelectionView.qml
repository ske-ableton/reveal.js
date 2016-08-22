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

Rectangle {
    id: root
    border.width: 1
    border.color: "chocolate"

    clip: true

    property var selectedGridItems
    property var activeInListView

    ItemSelectionModel {
        id: mySelectionModel
        model: root.selectedGridItems
        objectName: "itemSelectionModel" + root.num
        Component.onCompleted: {
            activeInListView.addSatellite(mySelectionModel);
        }
    }

    ListView {
        y: 25 + 33
        height: root.height - 50 - 33
        x: root.border.width
        width: root.width - root.border.width - root.border.width

        model: root.selectedGridItems

        clip: true

        delegate: Rectangle {
            width: ListView.view.width
            height: 25

            SelectionInfo {
                id: activeInfo
                selectionModel: mySelectionModel
                modelIndex: mySelectionModel.model.index(model.index, 0)
            }

            color: activeInfo.isCurrent ? "yellow" : "white"

            Text {
                anchors.fill: parent
                text: model.display
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    mySelectionModel.setCurrentIndex(mySelectionModel.model.index(model.index, 0),
                        ItemSelectionModel.ClearAndSelect);
                }
            }
        }
    }
}
