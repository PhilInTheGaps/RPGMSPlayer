import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0

Dialog {
    id: root
    title: qsTr("New")
    modal: true

    property alias subscenarioChecked: subscenario_check_box.checked
    property alias subscenarioIndex: subscenario_combo_box.currentIndex
    property bool canAccept: true

    function updateCanAccept() {
        if (!audio_editor) {
            canAccept = false
            return
        }

        var project = audio_editor.currentProject

        switch (combo_box.currentIndex) {
        case 0:
            canAccept = true
            break
        case 1:
            canAccept = project
            error_label.text = qsTr("No Project!")
            break
        case 2:
            canAccept = project && project.currentCategory
            error_label.text = qsTr("No Category!")
            break
        case 3:
        case 4:
        case 5:
            canAccept = project && project.currentScenario
            error_label.text = qsTr("No Scenario!")
            break
        default:
            canAccept = false
            error_label.text = qsTr("Unknown Error")
            break
        }
    }

    AddonElementDialog {
        id: addon_dialog

        width: editor_root.width * 0.75
        height: editor_root.height * 0.75
        x: (editor_root.width - width) / 2
    }

    contentItem: Column {
        spacing: 10

        Label {
            text: qsTr("Element Type")
        }

        CustomComboBox {
            id: combo_box
            model: [qsTr("Project"), qsTr("Category"), qsTr("Scenario"), qsTr(
                    "Music List"), qsTr("Sound List"), qsTr("Radio")]
            width: textfield.width

            onCurrentIndexChanged: {
                updateCanAccept()
            }
        }

        CustomCheckBox {
            id: subscenario_check_box
            text: (combo_box.currentIndex == 2 ? qsTr("As") : qsTr(
                                                     "In")) + " " + qsTr(
                      "Subscenario")
            checked: false
            enabled: canAccept
            visible: canAccept && combo_box.currentIndex > 1
            anchors.left: parent.left
            anchors.right: parent.right
        }

        CustomComboBox {
            id: subscenario_combo_box
            model: {
                if (audio_editor && audio_editor.currentProject
                        && audio_editor.currentProject.currentCategory
                        && audio_editor.currentProject.currentCategory.currentScenario) {

                    audio_editor.currentProject.currentCategory.currentScenario.scenarios
                } else {
                    []
                }
            }

            textRole: "name"
            visible: canAccept && combo_box.currentIndex > 2
                     && subscenario_check_box.checked
            anchors.left: parent.left
            anchors.right: parent.right
        }

        CustomButton {
            id: add_from_addons_button

            visible: canAccept && combo_box.currentIndex > 2

            anchors.left: parent.left
            anchors.right: parent.right
            enabled: combo_box.currentIndex == 6

            buttonText: qsTr("Add from Addons")
            onClicked: addon_dialog.open()
        }

        CustomTextField {
            id: textfield
            placeholderText: qsTr("Element Name")
            selectByMouse: true
            onAccepted: root.accept()
            enabled: canAccept
            visible: canAccept
        }

        Label {
            id: error_label
            visible: !canAccept

            text: qsTr("Unknown Error")
            leftPadding: error_icon.width + 10
            font.pointSize: 12

            Label {
                id: error_icon
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: 5

                text: FontAwesome.exclamationTriangle
                font.family: FontAwesome.familySolid
                font.pointSize: 12
                color: "red"
            }
        }
    }

    standardButtons: Dialog.Ok | Dialog.Cancel

    onOpened: {
        textfield.forceActiveFocus()
    }

    onAccepted: {
        if (textfield.text != "" && canAccept) {
            var subscenario = subscenario_check_box.checked ? subscenario_combo_box.currentIndex : -1

            switch (combo_box.currentIndex) {
            case 0:
                audio_editor.createProject(textfield.text)
                break
            case 1:
                audio_editor.createCategory(textfield.text)
                break
            case 2:
                audio_editor.createScenario(textfield.text,
                                            subscenario_check_box.checked)
                break
            case 3:
                audio_editor.createElement(textfield.text, 0, subscenario)
                break
            case 4:
                audio_editor.createElement(textfield.text, 1, subscenario)
                break
            case 5:
                audio_editor.createElement(textfield.text, 2, subscenario)
                break
            }
        }

        textfield.clear()
    }
}
