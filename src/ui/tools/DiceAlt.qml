import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4

Item {
    id: dice_page
    height: main_item.height

    Connections {
        target: dice_tool

        onMixedCriticalResult: roll_result.color = "orange"
        onSuccessfulCriticalResult: roll_result.color = "green"
        onFailedCriticalResult: roll_result.color = "red"
        onNormalResult: roll_result.color = color_scheme.textColor
    }

    Rectangle {
        id: main_item
        height: dice_row.height
        anchors.left: parent.left
        anchors.right: parent.right
        color: color_scheme.backgroundColor

        Row {
            id: dice_row
            spacing: 10
            padding: 10
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter

            // Dice Count
            SpinBox {
                id: count_spinbox
                value: 1
                from: 1
                onValueChanged: dice_tool.setAmount(value)
                editable: true
                font.pixelSize: parent.height * 0.4
            }

            Text {
                text: qsTr("D")
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: parent.height * 0.5
                font.bold: true
            }

            // Dice Type
            SpinBox {
                id: dice_type_spin_box
                value: 20
                to: 1000
                editable: true
                onValueChanged: dice_tool.setSides(value)
                font.pixelSize: parent.height * 0.4
            }

            Text {
                text: "+"
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: parent.height * 0.5
                font.bold: true
            }

            // Modifier
            SpinBox {
                value: 0
                from: -99
                to: 99
                onValueChanged: dice_tool.setModifier(value)
                editable: true
                font.pixelSize: parent.height * 0.4
            }

            // "Roll!"
            Button {
                Image {
                    source: "/icons/dice/dice_roll.png"
                    width: parent.height * 0.9
                    height: width

                    anchors.centerIn: parent

                    sourceSize.height: height
                    sourceSize.width: width
                }

                ToolTip.text: qsTr("Roll")
                ToolTip.visible: hovered
                hoverEnabled: true

                onClicked: {
                    roll_result.text = dice_tool.roll
                }
            }

            Text {
                text: qsTr(":")
                anchors.verticalCenter: parent.verticalCenter
                color: color_scheme.textColor
                font.pixelSize: parent.height * 0.5
                font.bold: true
            }

            // Result
            Text {
                id: roll_result
                text: "-"
                color: color_scheme.textColor
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: parent.height * 0.5
                font.bold: true
            }
        }
    }
}
