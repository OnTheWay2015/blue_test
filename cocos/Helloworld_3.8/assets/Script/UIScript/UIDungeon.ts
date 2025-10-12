import { _decorator, Prefab } from 'cc';
const {ccclass, property} = _decorator;

import UIDungeon_Auto from "../AutoScripts/UIDungeon_Auto";
import { Dungeon, GridType } from "../Common/Utils/Dungeon";
import FormMgr from "../UIFrame/FormMgr";
import { UIScreen } from "../UIFrame/UIForm";

@ccclass('UIDungeon')
export default class UIDungeon extends UIScreen {
    view: UIDungeon_Auto;
    private dungeon: Dungeon = null;
    start() {
        // this.view.Back.addClick(() => {
        // FormMgr.backScene();
        // }, this);
        // this.dungeon = new Dungeon(63, 63);
        // let map = this.dungeon.generate();
        // let size = this.pfGrid.data.width;
        // for(let j=0; j<this.dungeon.height; j++) {
        // for(let x=0; x<this.dungeon.width; x++) {
        // let node = cc.instantiate(this.pfGrid);
        // node.color = this.getColor(map[j * this.dungeon.width + x]);
        // this.view.Items.addChild(node);
        // node.setPosition(x * size, j * size);
        // }
        // }
    }
    private getColor(type: GridType) {
        // switch(type) {
        // case GridType.Wall:
        // return cc.Color.BLACK;
        // case GridType.Floor:
        // return cc.Color.WHITE;
        // case GridType.OpenDoor:
        // return cc.Color.BLUE;
        // case GridType.CloseDoor:
        // return cc.Color.RED;
        // }
    }
//    // update (dt) {}
}


/**
 * 注意：已把原脚本注释，由于脚本变动过大，转换的时候可能有遗落，需要自行手动转换
 */
// import UIDungeon_Auto from "../AutoScripts/UIDungeon_Auto";
// import { Dungeon, GridType } from "../Common/Utils/Dungeon";
// import FormMgr from "../UIFrame/FormMgr";
// import { UIScreen } from "../UIFrame/UIForm";
// 
// const {ccclass, property} = cc._decorator;
// 
// @ccclass
// export default class UIDungeon extends UIScreen {
// 
// 
//     @property(cc.Prefab) pfGrid: cc.Prefab = null;
// 
//     view: UIDungeon_Auto;
//     private dungeon: Dungeon = null;
// 
//     start() {
//         this.view.Back.addClick(() => {
//             FormMgr.backScene();
//         }, this);
//         this.dungeon = new Dungeon(63, 63);
//         let map = this.dungeon.generate();
//         let size = this.pfGrid.data.width;
//         for(let j=0; j<this.dungeon.height; j++) {
//             for(let x=0; x<this.dungeon.width; x++) {
//                 let node = cc.instantiate(this.pfGrid);            
//                 node.color = this.getColor(map[j * this.dungeon.width + x]);
//                 this.view.Items.addChild(node);
//                 node.setPosition(x * size, j * size);
//             }
//         }        
//     }
// 
//     private getColor(type: GridType) {
//         switch(type) {
//             case GridType.Wall:
//                 return cc.Color.BLACK;
//             case GridType.Floor:
//                 return cc.Color.WHITE;
//             case GridType.OpenDoor:
//                 return cc.Color.BLUE;
//             case GridType.CloseDoor:
//                 return cc.Color.RED;
//         }        
//     }
// 
// 
//     // update (dt) {}
// }
