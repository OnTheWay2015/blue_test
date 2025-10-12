import { _decorator } from 'cc';
const {ccclass, property} = _decorator;

import UIECSView_Auto from "../AutoScripts/UIECSView_Auto";
import { UIScreen } from "../UIFrame/UIForm";

@ccclass('UIECSView')
export default class UIECSView extends UIScreen {
    view: UIECSView_Auto;
//    // onLoad () {}
    start () {

    }
//    // update (dt) {}
}


/**
 * 注意：已把原脚本注释，由于脚本变动过大，转换的时候可能有遗落，需要自行手动转换
 */
// import UIECSView_Auto from "../AutoScripts/UIECSView_Auto";
// import { UIScreen } from "../UIFrame/UIForm";
// 
// const {ccclass, property} = cc._decorator;
// 
// @ccclass
// export default class UIECSView extends UIScreen {
// 
//     view: UIECSView_Auto;
//     // onLoad () {}
// 
//     start () {
// 
//     }
// 
//     // update (dt) {}
// }
