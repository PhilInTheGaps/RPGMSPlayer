import QtQuick 2.6
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0

import "./buttons"
import "../../defines.js" as Defines

Rectangle {
    id: left_menu
    color: palette.dark

    signal editorButtonClicked

    Item {
        id: top_bar

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: Defines.TOOLBAR_HEIGHT

        anchors.leftMargin: 5
        anchors.rightMargin: 5

        // Project ComboBox
        CustomToolBarComboBox {
            id: audio_project_combo_box
            property int loaded: 0

            anchors.left: parent.left
            anchors.right: editor_button.left

            width: parent.width - editor_button.width - parent.spacing
            model: audio_tool ? audio_tool.projects : []
            textRole: "name"
            emptyString: loaded > 0 ? qsTr("No Projects") : qsTr("Loading ...")

            onCurrentTextChanged: {
                if (audio_tool) {
                    audio_tool.setCurrentProject(currentIndex)
                }
            }

            Connections {
                target: audio_tool

                function onProjectsChanged() {
                    audio_project_combo_box.loaded += 1
                }
            }
        }

        // Open Editor Button
        CustomToolBarButton {
            id: editor_button
            iconText: FontAwesome.edit

            anchors.right: parent.right
            anchors.topMargin: 8
            anchors.bottomMargin: 8

            onClicked: editorButtonClicked()
        }
    }

    ScrollView {
        id: audio_project_menu

        anchors.top: top_bar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: sound_column.top
        anchors.margins: 5

        clip: true
        contentWidth: -1
        contentHeight: audio_project_structure.implicitHeight

        Column {
            id: audio_project_structure
            anchors.left: parent.left
            anchors.right: parent.right

            Repeater {
                id: category_repeater

                model: audio_tool
                       && audio_tool.currentProject ? audio_tool.currentProject.categories : []

                CustomButton {
                    buttonText: modelData.name

                    anchors.left: parent.left
                    anchors.right: parent.right

                    onClicked: {
                        audio_tool.currentProject.setCurrentCategory(modelData)
                    }
                }
            }
        }
    }

    Column {
        id: sound_column
        anchors.bottom: cover_image.top
        anchors.left: parent.left
        anchors.right: parent.right
        spacing: 5
        bottomPadding: 5

        Repeater {
            id: sound_repeater
            model: audio_tool ? audio_tool.soundController.activeElements : []

            SoundButton {
                element: modelData.name
                element_icon: modelData.icon
                anchors.left: parent.left
                anchors.right: parent.right
            }
        }
    }

    Image {
        id: cover_image
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        visible: source != ""
        sourceSize.width: width
        source: audio_tool ? audio_tool.metaData.cover : ""
    }
}
