import { _decorator } from 'cc';
const {ccclass, property} = _decorator;

import UIPop_Auto from "../AutoScripts/UIPop_Auto";
import { ModalOpacity } from "../UIFrame/config/SysDefine";
import { ModalType } from "../UIFrame/Struct";
import { UIWindow } from "../UIFrame/UIForm";

@ccclass('UIPop')
export default class UIPop extends UIWindow {
    modalType: ModalType = new ModalType(ModalOpacity.OpacityHalf);
    view: UIPop_Auto;
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
// import UIPop_Auto from "../AutoScripts/UIPop_Auto";
// import { ModalOpacity } from "../UIFrame/config/SysDefine";
// import { ModalType } from "../UIFrame/Struct";
// import { UIWindow } from "../UIFrame/UIForm";
// 
// const {ccclass, property} = cc._decorator;
// 
// @ccclass
// export default class UIPop extends UIWindow {
// 
//     modalType: ModalType = new ModalType(ModalOpacity.OpacityHalf);
// 
//     view: UIPop_Auto;
// 
//     // onLoad () {}
// 
//     start () {
//         this.view.Close.addClick(() => {
//             this.closeSelf();
//         }, this);
//     }
// 
// 
//     // update (dt) {}
// }
