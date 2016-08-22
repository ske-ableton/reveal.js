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

    MyModel {
        id: myModel
    }

    ItemSelectionModel {
        id: itemSelection
        model: myModel
    }

    property var trackSelection

    ListView {
        anchors.fill: parent
        anchors.margins: 1

        model: myModel

        clip: true

        delegate: Rectangle {
            height: 30
            width: 150

            color: model.decoration

            SelectionInfo {
                id: selectionInfo
                selectionModel: itemSelection
                modelIndex: itemSelection.model.index(model.index, 0)
            }

            Rectangle {
                width: 20
                height: 20
                x: 5
                y: 5
                color: selectionInfo.isSelected ? "green" : "white"
            }

            Text {
                anchors.centerIn: parent
                text: model.display
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    itemSelection.select(itemSelection.model.index(model.index, 0),
                        ItemSelectionModel.Toggle);
                }
            }
        }
    }
}
