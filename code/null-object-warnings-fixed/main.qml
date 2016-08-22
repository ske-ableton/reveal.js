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
import QtQuick.Window 2.2
import QtQuick.Controls 1.4

import Music 1.0

Window {
    id: root

    visible: true
    width: 400
    height: 400

    readonly property real slowDuration: 3000
    readonly property real fastDuration: 500

    property real animationDuration: root.fastDuration

    Row {
        y: 20
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 10

        Button {
            text: "Reset list!"
            onClicked: _viewNonQtEntryPoint.resetList();
        }

        Button {
            text: root.animationDuration === root.slowDuration ? "Fast!" : "Slow!"
            onClicked: {
                root.animationDuration =
                    (root.animationDuration === root.slowDuration)
                        ? root.fastDuration
                        : root.slowDuration
            }
        }
    }

    property int baselineValue

    NumberAnimation on baselineValue {
        loops: Animation.Infinite
        from: 0
        to: 1
        duration: root.animationDuration
    }

    ListView {
        id: theListView
        anchors.horizontalCenter: parent.horizontalCenter
        y: 100
        width: 250
        height: 300
        spacing: 2

        remove: Transition {
            ParallelAnimation {
                NumberAnimation { property: "opacity"; to: 0; duration: root.animationDuration }
                NumberAnimation { properties: "x"; to: 100; duration: root.animationDuration }
            }
        }

        displaced: Transition {
            NumberAnimation { properties: "y"; duration: root.animationDuration }
        }

        model: _viewNonQtEntryPoint.meterList

        delegate: ValueDisplayMeter {
            id: theDelegate
            Meter {
                id: meter
                modelIndex: theDelegate.ListView.view.model.index(model.index, 0)
            }
            // The delegate binds to some value from outside the delegate
            value: root.baselineValue + meter.value
        }
    }
}
