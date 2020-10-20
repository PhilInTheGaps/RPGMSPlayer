import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.impl 2.2
import QtQuick.Templates 2.2 as T
import "../colors.js" as Colors

T.ScrollIndicator {
    id: control

    implicitWidth: Math.max(
                       background ? background.implicitWidth : 0,
                       contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(
                        background ? background.implicitHeight : 0,
                        contentItem.implicitHeight + topPadding + bottomPadding)

    padding: 2

    contentItem: Rectangle {
        implicitWidth: 2
        implicitHeight: 2

        color: Colors.button
        visible: control.size < 1.0
        opacity: 0.0

        states: State {
            name: "active"
            when: control.active
            PropertyChanges {
                target: control.contentItem
                opacity: 0.75
            }
        }

        transitions: [
            Transition {
                from: "active"
                SequentialAnimation {
                    PauseAnimation {
                        duration: 450
                    }
                    NumberAnimation {
                        target: control.contentItem
                        duration: 200
                        property: "opacity"
                        to: 0.0
                    }
                }
            }
        ]
    }
}
