import QtQuick 2.9
import QtQuick.Controls 2.2

import "../../../components"

Item {
    DSA5ItemList {
        anchors.fill: parent
        model: dsa5TalentsModel
        value_fields: 4
        spacer_indices: [13, 22, 29, 41]
    }
}
