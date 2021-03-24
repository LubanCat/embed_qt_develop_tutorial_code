import QtQuick 2.11
import QtQuick.Window 2.11

Window {
    visible: true
    width: 640
    height: 480
    property alias text1: text1
    title: qsTr("Hello World")

    Text {
        id: text1
        text: qsTr("hello quick")
        anchors.fill: parent
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        elide: Text.ElideNone
        font.pixelSize: 12
    }
}

/*##^## Designer {
    D{i:2;anchors_height:107;anchors_width:269;anchors_x:186;anchors_y:187}
}
 ##^##*/
