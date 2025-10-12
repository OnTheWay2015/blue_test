import { _decorator } from 'cc';
const {ccclass, property} = _decorator;

import UISkills_Auto from "../AutoScripts/UISkills_Auto";
import { UIWindow } from "../UIFrame/UIForm";

@ccclass('UISkills')
export default class UISkills extends UIWindow {
    view: UISkills_Auto;
//    // onLoad () {}
    start () {
        // this.view.Close.addClick(() => {
        // this.closeSelf();
        // }, this);
    }
//    // update (dt) {}
}


/**
 * 注意：已把原脚本注释，由于脚本变动过大，转换的时候可能有遗落，需要自行手动转换
 */
// import UISkills_Auto from "../AutoScripts/UISkills_Auto";
// import { UIWindow } from "../UIFrame/UIForm";
// 
// const {ccclass, property} = cc._decorator;
// 
// @ccclass
// export default class UISkills extends UIWindow {
// 
//     view: UISkills_Auto;
// 
//     // onLoad () {}
// 
//     start () {
//         this.view.Close.addClick(() => {
//             this.closeSelf();
//         }, this);
//     }
// 
//     // update (dt) {}
// }
