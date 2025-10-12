import { _decorator } from 'cc';
const {ccclass, property} = _decorator;

import UISetting_Auto from "../AutoScripts/UISetting_Auto";
import AdapterMgr from "../UIFrame/AdapterMgr";
import { ModalOpacity } from "../UIFrame/config/SysDefine";
import FormMgr from "../UIFrame/FormMgr";
import { ModalType } from "../UIFrame/Struct";
import { UIWindow } from "../UIFrame/UIForm";
import WindowMgr from "../UIFrame/WindowMgr";;

@ccclass('UISetting')
export default class UISetting extends UIWindow {
    view: UISetting_Auto;
    modalType = new ModalType(ModalOpacity.OpacityHalf, true);
//    // onLoad () {}
    start () {

        // this.view.Pop.addClick(() => {
        // FormMgr.open(UIConfig.UIPop);
//            // FormMgr.open(UIConfig.UITips, "关闭后才显示的弹窗1", {showWait: true});
//            // FormMgr.open(UIConfig.UITips, "关闭后才显示的弹窗2", {showWait: true})
        // }, this);

        // this.view.Mobx.addClick(() => {
        // FormMgr.open(UIConfig.UIMobx);
        // }, this);

        // this.view.Capture.addClick(() => {
        // FormMgr.open(UIConfig.UICapture);
        // }, this);

        // this.view.Light.addClick(() => {
        // FormMgr.open(UIConfig.UILight);
        // }, this);
    }
    async showEffect() {
        // let len = AdapterMgr.inst.visibleSize.height/2 + 300;
        // this.node.y = len;
        // await CocosHelper.runTweenSync(this.node, cc.tween().to(0.5, {y: 0}, cc.easeBackOut()));
    }
    async hideEffect() {
        // let len = AdapterMgr.inst.visibleSize.height/2 + 300;
        // this.node.y = 0;
        // await CocosHelper.runTweenSync(this.node, cc.tween().to(0.3, {y: len}, cc.easeBackIn()));
    }
//    // update (dt) {}
}


/**
 * 注意：已把原脚本注释，由于脚本变动过大，转换的时候可能有遗落，需要自行手动转换
 */
// import UISetting_Auto from "../AutoScripts/UISetting_Auto";
// import AdapterMgr from "../UIFrame/AdapterMgr";
// import CocosHelper from "../UIFrame/CocosHelper";
// import { ModalOpacity } from "../UIFrame/config/SysDefine";
// import FormMgr from "../UIFrame/FormMgr";
// import { ModalType } from "../UIFrame/Struct";
// import { UIWindow } from "../UIFrame/UIForm";
// import WindowMgr from "../UIFrame/WindowMgr";;
// import UIConfig from "./../UIConfig";
// 
// const {ccclass, property} = cc._decorator;
// 
// @ccclass
// export default class UISetting extends UIWindow {
//     view: UISetting_Auto;
// 
//     modalType = new ModalType(ModalOpacity.OpacityHalf, true);
// 
//     // onLoad () {}
// 
//     start () {
// 
//         this.view.Pop.addClick(() => {
//             FormMgr.open(UIConfig.UIPop);
//             // FormMgr.open(UIConfig.UITips, "关闭后才显示的弹窗1", {showWait: true});
//             // FormMgr.open(UIConfig.UITips, "关闭后才显示的弹窗2", {showWait: true})
//         }, this);
// 
//         this.view.Mobx.addClick(() => {
//             FormMgr.open(UIConfig.UIMobx);
//         }, this);
// 
//         this.view.Capture.addClick(() => {
//             FormMgr.open(UIConfig.UICapture);
//         }, this);
// 
//         this.view.Light.addClick(() => {
//             FormMgr.open(UIConfig.UILight);
//         }, this);
//     }
// 
//     async showEffect() {
//         let len = AdapterMgr.inst.visibleSize.height/2 + 300;
//         this.node.y = len;
//         await CocosHelper.runTweenSync(this.node, cc.tween().to(0.5, {y: 0}, cc.easeBackOut()));
//     }
// 
//     async hideEffect() {
//         let len = AdapterMgr.inst.visibleSize.height/2 + 300;
//         this.node.y = 0;
//         await CocosHelper.runTweenSync(this.node, cc.tween().to(0.3, {y: len}, cc.easeBackIn()));
//     }
// 
//     // update (dt) {}
// }
