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
    objectName: "theGridItems"
    border.width: 1
    border.color: "red"

    property var myModel

    property var activeGridItemSelections
    property var activeGridItems

    CrosshairSelection {
        id: crosshair
        objectName: "crosshair"
        selectionModel: root.activeGridItems
    }

    ListView {
        anchors.fill: parent
        anchors.margins: parent.border.width

        model: root.myModel

        clip: true

        orientation: ListView.Horizontal

        delegate: Item {
            id: theDelegate
            objectName: "theDelegate"
            height: ListView.view.height
            width: 94

            ChildListFilter {
                id: childrenFilter
                modelIndex: theDelegate.ListView.view.model.index(model.index, 0)
                sourceModel: theDelegate.ListView.view.model
            }

            LinkItemSelectionModel {
                id: activeSelectionModel
                model: childrenFilter
                linkedItemSelectionModel: activeGridItems
            }

            // NOTE: Move this out of the delegate
            // This represents view state, so when the delegate is destroyed,
            // we will lose the selection here. At least in the design in this
            // testcase. An alternative is possible.
            ItemSelectionModel {
                id: localSelection
                model: childrenFilter
                Component.onCompleted: {
                    root.activeGridItemSelections.addSatellite(localSelection);
                }
            }

            LinkItemSelectionModel {
                id: localCrosshairSelectionModel
                model: childrenFilter
                linkedItemSelectionModel: crosshair
            }

            ListView {
                width: theDelegate.width
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                model: childrenFilter

                delegate: Rectangle {
                    height: 25
                    width: theDelegate.width
                    color: {
                        if (gridItemSelectionInfo.isSelected) {
                            "yellow";
                        } else if (crosshairInfo.isSelected) {
                            Qt.darker("lightsteelblue");
                        } else {
                            "lightsteelblue";
                        }
                    }

                    SelectionInfo {
                        id: gridItemSelectionInfo
                        selectionModel: localSelection
                        modelIndex: localSelection.model.index(model.index, 0)
                    }

                    SelectionInfo {
                        id: crosshairInfo
                        selectionModel: localCrosshairSelectionModel
                        modelIndex: localCrosshairSelectionModel.model.index(model.index, 0)
                    }

                    Text {
                        text: model.display
                        anchors.centerIn: parent
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            var action = ItemSelectionModel.Select;
                            if (mouse.modifiers & Qt.ControlModifier) {
                                action = ItemSelectionModel.Deselect;
                            }
                            localSelection.select(localSelection.model.index(model.index, 0), action)
                        }
                    }
                }
            }
        }
    }
}
