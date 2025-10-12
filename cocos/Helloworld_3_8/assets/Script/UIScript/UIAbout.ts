import { _decorator } from 'cc';
const {ccclass, property} = _decorator;

import UIAbout_Auto from "../AutoScripts/UIAbout_Auto";
import FormMgr from "../UIFrame/FormMgr";
import { UIScreen } from "../UIFrame/UIForm";

@ccclass('UIAbout')
export default class UIAbout extends UIScreen {
    view: UIAbout_Auto;
//    // LIFE-CYCLE CALLBACKS:
//    // onLoad () {}
    start () {
        // this.view.Close.addClick(() => {
        // FormMgr.backScene();
        // }, this);
    }
//    // update (dt) {}
}


/**
 * 注意：已把原脚本注释，由于脚本变动过大，转换的时候可能有遗落，需要自行手动转换
 */
// import UIAbout_Auto from "../AutoScripts/UIAbout_Auto";
// import FormMgr from "../UIFrame/FormMgr";
// import { UIScreen } from "../UIFrame/UIForm";
// 
// const {ccclass, property} = cc._decorator;
// 
// @ccclass
// export default class UIAbout extends UIScreen {
// 
//     view: UIAbout_Auto;
// 
//     // LIFE-CYCLE CALLBACKS:
// 
//     // onLoad () {}
// 
//     start () {
//         this.view.Close.addClick(() => {
//             FormMgr.backScene();
//         }, this);
//     }
// 
//     // update (dt) {}
// }
