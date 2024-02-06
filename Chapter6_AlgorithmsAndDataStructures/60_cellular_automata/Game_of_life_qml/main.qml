import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import QtQml 2.0
import cellularAutomata.gameOfLife 1.0

Window {
    GameOfLife{id:game_of_life_obj}
    id: window
    visible: true
    width: 750
    height: 650
    maximumHeight: height
    maximumWidth: width
    minimumHeight: height
    minimumWidth: width
    opacity: 1
    title: qsTr("Cellular Automata")
    Connections{
        target: game_of_life_obj
    }

    Grid{
        id: grid
        x: 125
        y: 28
        width: 500
        height: 500
        verticalItemAlignment: Grid.AlignVCenter
        horizontalItemAlignment: Grid.AlignHCenter
        leftPadding: 0
        transformOrigin: Item.Center
        z: 0
        rotation: 0
        spacing: 0
        rows: 35
        columns: 35

        flow: Grid.LeftToRight
        Repeater{
            model: grid.rows*grid.columns
            id:rptr
            width: 500
            height: 500
            Rectangle
            {
                property int idx: index
                property bool selected: false
                color: selected?"black":"white"//(configuration.currentText=="All alive") ? "black" : "white";
                width: 15; height: 15; radius: 0; border.color: "gray";border.width:1;
                MouseArea{
                    visible: true
                    anchors.fill:parent
                    onClicked:{
                        parent.selected=!parent.selected;
                        game_of_life_obj.set_state_of_cell(idx,parent.selected);
                        console.log(idx);
                    }
                }
            }
        }
    }
    function update_grid()
    {
        game_of_life_obj.recalculate();
        update_grid_from_back_end();
    }
    function update_grid_from_back_end()
    {
        for(var i=0;i<rptr.count;++i)
        {
            rptr.itemAt(i).selected=game_of_life_obj.state_of(i);
        }
    }

    Timer{
        id:timer
        running: false
        repeat: true
        interval: 100
        onTriggered: {update_grid();}
    }

    Button {
        id: button
        x: 325
        y: 584
        width: 100
        height: 40
        text: timer.running ? "❙❙" : "▶️"
        font.pointSize: 14
        onClicked: timer.running=!timer.running;
    }

    Button {
        id: clear_all_btn
        enabled: !timer.running
        height: 40
        text: qsTr("CLEAR ALL")
        anchors.left: parent.left
        anchors.right: grid.left
        anchors.top: parent.top
        anchors.bottom: set_all_btn.top
        anchors.bottomMargin: 28
        anchors.topMargin: 185
        anchors.rightMargin: 13
        anchors.leftMargin: 13
        onClicked:
        {
            game_of_life_obj.clearAll();
            update_grid_from_back_end();
        }
    }

    Button {
        id: set_all_btn
        enabled: !timer.running
        y: 264
        text: qsTr("SET ALL")
        anchors.left: parent.left
        anchors.right: grid.left
        anchors.bottom: toggle_btn.top
        anchors.bottomMargin: 47
        anchors.rightMargin: 13
        anchors.leftMargin: 13
        onClicked: {
            game_of_life_obj.setAll();
            update_grid_from_back_end();
        }
    }

    Button {
        id: toggle_btn
        enabled: !timer.running
        y: 351
        text: qsTr("TOGGLE")
        anchors.left: parent.left
        anchors.right: grid.left
        anchors.bottom: randmz_btn.top
        anchors.bottomMargin: 47
        anchors.rightMargin: 13
        anchors.leftMargin: 13
        onClicked: {
            for(var i=0;i<rptr.count;++i)
            {
                rptr.itemAt(i).selected= !rptr.itemAt(i).selected;
                game_of_life_obj.set_state_of_cell(i,rptr.itemAt(i).selected);
            }
        }
    }

    Button {
        id: randmz_btn
        enabled: !timer.running
        y: 431
        text: qsTr("RANDOMIZE")
        anchors.left: parent.left
        anchors.right: grid.left
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 179
        anchors.leftMargin: 13
        anchors.rightMargin: 13
        onClicked: {
            game_of_life_obj.randomize_cells();
            update_grid_from_back_end();
        }
    }
    Component.onCompleted:
    {
        game_of_life_obj.set_grid_dimension(grid.rows,grid.columns);
        update_grid_from_back_end();
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.6600000262260437}D{i:3}D{i:8}
}
##^##*/
