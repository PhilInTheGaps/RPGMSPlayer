import QtQuick 2.9
import QtQuick.Controls 2.2

Dialog {
    id: root
    title: qsTr("Dice Settings")
    modal: true

    contentItem: Column {
        padding: 5
        spacing: 5

        CheckBox {
            id: enable_criticals_checkbox
            text: qsTr("Enable Criticals")
            checked: dice_tool.getCriticalEnabled()
        }

        RadioButton {
            id: min_max_radiobutton
            text: qsTr("Min / Max")
            checked: dice_tool.getMinMax()
        }

        Column {
            padding: 10
            spacing: 5

            RadioButton {
                id: success_max_radiobutton
                text: qsTr("Success: Maximum, Failure: Minimum")
                checked: dice_tool.getSuccessMax()
            }

            RadioButton {
                text: qsTr("Success: Minimum, Failure: Maximum")
                checked: !dice_tool.getSuccessMax()
            }
        }

        RadioButton {
            text: qsTr("Fixed Values")
            checked: !dice_tool.getMinMax()
        }

        Column {
            padding: 10
            spacing: 5

            Text {
                text: qsTr("Critical Success") + ":"
            }

            SpinBox {
                id: success_spinbox
                from: 0
                to: 1000
                value: dice_tool.getSuccess()
                editable: true
            }

            Text {
                text: qsTr("Critical Failure") + ":"
            }

            SpinBox {
                id: failure_spinbox
                from: 0
                to: 1000
                value: dice_tool.getFailure()
                editable: true
            }
        }
    }

    standardButtons: Dialog.Ok | Dialog.Cancel

    onAccepted: {
        dice_tool.setDiceSettings(enable_criticals_checkbox.checked,
                                  success_spinbox.value, failure_spinbox.value,
                                  min_max_radiobutton.checked,
                                  success_max_radiobutton.checked)
    }
}
