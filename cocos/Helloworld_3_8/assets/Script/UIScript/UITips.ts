import { _decorator } from 'cc';
const {ccclass, property} = _decorator;

import UITips_Auto from "../AutoScripts/UITips_Auto";
import { ModalOpacity } from "../UIFrame/config/SysDefine";
import { ModalType } from "../UIFrame/Struct";
import { UIWindow } from "../UIFrame/UIForm";

@ccclass('UITips')
export default class UITips extends UIWindow {
    modalType = new ModalType(ModalOpacity.OpacityHalf, true);
    view: UITips_Auto;
    
//    // onLoad () {}
    start () {

    }
    onShow(str: string) {
        // this.view.Tips.string = str;
    }
//    // update (dt) {}
}


/**
 * 注意：已把原脚本注释，由于脚本变动过大，转换的时候可能有遗落，需要自行手动转换
 */
// import UITips_Auto from "../AutoScripts/UITips_Auto";
// import { ModalOpacity } from "../UIFrame/config/SysDefine";
// import { ModalType } from "../UIFrame/Struct";
// import { UIWindow } from "../UIFrame/UIForm";
// 
// const {ccclass, property} = cc._decorator;
// 
// @ccclass
// export default class UITips extends UIWindow {
// 
//     modalType = new ModalType(ModalOpacity.OpacityHalf, true);
// 
//     view: UITips_Auto;
//     
// 
//     // onLoad () {}
// 
//     start () {
// 
//     }
// 
//     onShow(str: string) {
//         this.view.Tips.string = str;
//     }
// 
//     // update (dt) {}
// }
