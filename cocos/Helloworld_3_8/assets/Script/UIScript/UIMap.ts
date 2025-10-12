import { _decorator } from 'cc';
const {ccclass, property} = _decorator;

import UIMap_Auto from "../AutoScripts/UIMap_Auto";
import FormMgr from "../UIFrame/FormMgr";
import { UIScreen } from "../UIFrame/UIForm";
import  "./../UIConfig";

@ccclass('UIMap')
export default class UIMap extends UIScreen {
    view: UIMap_Auto;
//    // LIFE-CYCLE CALLBACKS:
//    // onLoad () {}
    async load() {
        // await FormMgr.open(UIConfig.UIFunction);
        return '';
    }
    start () {
        // this.view.Round.addClick(() => {
        // FormMgr.open(UIConfig.UIGame);
        // }, this);
        // this.view.Back.addClick(() => {
        // FormMgr.backScene();
        // }, this);
    }
//    // update (dt) {}
}


/**
 * 注意：已把原脚本注释，由于脚本变动过大，转换的时候可能有遗落，需要自行手动转换
 */
// import UIMap_Auto from "../AutoScripts/UIMap_Auto";
// import FormMgr from "../UIFrame/FormMgr";
// import { UIScreen } from "../UIFrame/UIForm";
// import UIConfig from "./../UIConfig";
// 
// const {ccclass, property} = cc._decorator;
// 
// @ccclass
// export default class UIMap extends UIScreen {
// 
//     view: UIMap_Auto;
// 
//     // LIFE-CYCLE CALLBACKS:
// 
//     // onLoad () {}
// 
//     async load() {
//         await FormMgr.open(UIConfig.UIFunction);
//         return '';
//     }
// 
//     start () {
//         this.view.Round.addClick(() => {
//             FormMgr.open(UIConfig.UIGame);
//         }, this);
//         this.view.Back.addClick(() => {
//             FormMgr.backScene();
//         }, this);
//     }
// 
//     // update (dt) {}
// }
