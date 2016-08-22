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

import QtQuick 2.5

Rectangle {
    id: root
    width: 250
    height: 50
    border.color: "black"
    color: "transparent"

    property real value

    Rectangle {
        width : root.value

        Behavior on width {
            NumberAnimation {
                duration: 500
            }
        }

        x: 5
        anchors.verticalCenter: parent.verticalCenter
        height: 20
        color: Qt.rgba(Math.random(), Math.random(), Math.random(), Math.random())
    }

    Text {
        anchors.centerIn: parent
        text: (root.value * 100 / root.width).toFixed(2) + "%"
    }

    Rectangle {
        color: "red"
        width: 15
        height: 15
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.verticalCenter: parent.verticalCenter

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (model.index >= 0)
                    _viewNonQtEntryPoint.removeMeter(model.index)
            }
        }
    }
}
