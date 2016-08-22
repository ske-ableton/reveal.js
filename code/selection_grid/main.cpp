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

#include <QApplication>
#include <QQuickView>

#include "introspectionwidget.h"

#include "selectionaggregator.h"
#include "childlistfilter.h"
#include "crosshairselection.h"
#include "mymodel.h"
#include "selectioninfo.h"

#include <KSelectionProxyModel>
#include <KLinkItemSelectionModel>
#include "SortFilterProxyModel.hpp"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    qmlRegisterType<SelectionAggregator>("MyLib", 1, 0, "SelectionAggregator");
    qmlRegisterType<ChildListFilter>("MyLib", 1, 0, "ChildListFilter");
    qmlRegisterType<CrosshairSelection>("MyLib", 1, 0, "CrosshairSelection");
    qmlRegisterType<MyModel>("MyLib", 1, 0, "MyModel");
    qmlRegisterType<SelectionInfo>("MyLib", 1, 0, "SelectionInfo");

    qmlRegisterType<KSelectionProxyModel>("MyLib", 1, 0, "SelectionProxyModel");
    qmlRegisterType<KLinkItemSelectionModel>("MyLib", 1, 0, "LinkItemSelectionModel");
    qmlRegisterType<SortFilterProxyModel>("MyLib", 1, 0, "SortFilterProxyModel");

    QQuickView quickView;

    IntrospectionWidget iw(&quickView);
    iw.show();

    quickView.setSource(QUrl(SRC_DIR "/qml/main.qml"));
    quickView.show();

    return app.exec();
}
