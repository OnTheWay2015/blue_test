import { _decorator } from 'cc';
const {ccclass, property} = _decorator;

import UIAbout_Auto from "../AutoScripts/UIAbout_Auto";
import UIDrag_Auto from "../AutoScripts/UIDrag_Auto";
import FormMgr from "../UIFrame/FormMgr";
import { UIScreen } from "../UIFrame/UIForm";

@ccclass('UIDrag')
export default class UIDrag extends UIScreen {
    view:UIDrag_Auto;
//    // LIFE-CYCLE CALLBACKS:
    onLoad () {}
    start () {
        // this.view.about_up.addClick(() => {
        // FormMgr.backScene();
        // }, this);
    }
//    // update (dt) {}
}


/**
 * 注意：已把原脚本注释，由于脚本变动过大，转换的时候可能有遗落，需要自行手动转换
 */
// import UIAbout_Auto from "../AutoScripts/UIAbout_Auto";
// import UIDrag_Auto from "../AutoScripts/UIDrag_Auto";
// import FormMgr from "../UIFrame/FormMgr";
// import { UIScreen } from "../UIFrame/UIForm";
// 
// const {ccclass, property} = cc._decorator;
// 
// @ccclass
// export default class UIDrag extends UIScreen {
// 
//     view:UIDrag_Auto;
// 
//     // LIFE-CYCLE CALLBACKS:
// 
//     onLoad () {}
// 
//     start () {
//         this.view.about_up.addClick(() => {
//             FormMgr.backScene();
//         }, this);
//     }
// 
//     // update (dt) {}
// }
