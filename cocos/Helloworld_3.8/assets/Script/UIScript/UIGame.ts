import { _decorator } from 'cc';
const {ccclass, property} = _decorator;

//import UIGame_Auto from "../AutoScripts/UIGame_Auto";
import FormMgr from "../UIFrame/FormMgr";
import { UIScreen } from "../UIFrame/UIForm";

@ccclass('UIGame')
export default class UIGame extends UIScreen {
    //view: UIGame_Auto;
//    // onLoad () {}
    start () {
        // this.view.Back.addClick(() => {
        // FormMgr.backScene();
        // }, this);
    }
//    // update (dt) {}
}


/**
 * 注意：已把原脚本注释，由于脚本变动过大，转换的时候可能有遗落，需要自行手动转换
 */
// import UIGame_Auto from "../AutoScripts/UIGame_Auto";
// import FormMgr from "../UIFrame/FormMgr";
// import { UIScreen } from "../UIFrame/UIForm";
// 
// const {ccclass, property} = cc._decorator;
// 
// @ccclass
// export default class UIGame extends UIScreen {
// 
// 
//     view: UIGame_Auto;
// 
//     // onLoad () {}
// 
//     start () {
//         this.view.Back.addClick(() => {
//             FormMgr.backScene();
//         }, this);
//     }
// 
//     // update (dt) {}
// }
