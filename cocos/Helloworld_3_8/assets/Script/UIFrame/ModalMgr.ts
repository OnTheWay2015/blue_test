// /**
//  * 遮罩管理
//  */

import { _decorator, Component,Node,find, UITransform } from 'cc';
const {ccclass, property} = _decorator;

import { SysDefine } from "./config/SysDefine";
import UIModalScript from "./UIModalScript";
import { ModalType } from "./Struct";
import { UIWindow } from "./UIForm";

@ccclass('ModalMgr')
export default class ModalMgr extends Component {
    public static popUpRoot = '';
    public static _inst: ModalMgr = null;
    public static get inst() {
        if(this._inst == null) {
        this._inst = new ModalMgr();

        let node = new Node();
        node.name = "UIModalNode";
        ModalMgr.popUpRoot = SysDefine.SYS_UIROOT_NAME + '/' + SysDefine.SYS_POPUP_NODE

        let rootNode = find(ModalMgr.popUpRoot);
        rootNode.addChild(node);
        this._inst.uiModal = node.addComponent(UIModalScript);
        this._inst.uiModal.init();
        }
        return this._inst;
    }
    private uiModal:UIModalScript = null;
//    /** 为mask添加颜色 */
    private async showModal(maskType: ModalType) {
        await this.uiModal.showModal(maskType.opacity, maskType.easingTime, maskType.isEasing, maskType.dualBlur);
    }
    public checkModalWindow(coms: UIWindow[]) {
        if (coms.length <= 0) {
            this.uiModal.node.active = false;
            return;
        }
        this.uiModal.node.active = true;
        if (this.uiModal.node.parent) {
            this.uiModal.node.removeFromParent();
        }
        for (let i = coms.length - 1; i >= 0; i--) {
            if (coms[i].modalType.opacity > 0) {
                const uiTransform = coms[i].node.getComponent(UITransform);
                const result = Math.max(uiTransform.priority - 1, 0);
                uiTransform.priority = result;
                find(ModalMgr.popUpRoot).addChild(this.uiModal.node);
                this.uiModal.fid = coms[i].fid;
                this.showModal(coms[i].modalType);
                break;
            }
        }
    }
}


/**
 * 注意：已把原脚本注释，由于脚本变动过大，转换的时候可能有遗落，需要自行手动转换
 */
// import { SysDefine } from "./config/SysDefine";
// import UIModalScript from "./UIModalScript";
// import { ModalType } from "./Struct";
// import { UIWindow } from "./UIForm";
// 
// /**
//  * 遮罩管理
//  */
// const {ccclass, property} = cc._decorator;
// 
// @ccclass
// export default class ModalMgr extends cc.Component {
//     public static popUpRoot = '';
//     public static _inst: ModalMgr = null;
//     public static get inst() {
//         if(this._inst == null) {
//             this._inst = new ModalMgr();
// 
//             let node = new cc.Node("UIModalNode");
//             ModalMgr.popUpRoot = SysDefine.SYS_UIROOT_NAME + '/' + SysDefine.SYS_POPUP_NODE
// 
//             let rootNode = cc.find(ModalMgr.popUpRoot);
//             rootNode.addChild(node);
//             this._inst.uiModal = node.addComponent(UIModalScript);
//             this._inst.uiModal.init();
//         }
//         return this._inst;
//     }
// 
//     private uiModal:UIModalScript = null;
// 
//     /** 为mask添加颜色 */
//     private async showModal(maskType: ModalType) {
//         await this.uiModal.showModal(maskType.opacity, maskType.easingTime, maskType.isEasing, maskType.dualBlur);
//     }
// 
//     public checkModalWindow(coms: UIWindow[]) {
//         if(coms.length <= 0) {
//             this.uiModal.node.active = false;
//             return ;
//         }
//         this.uiModal.node.active = true;
//         if(this.uiModal.node.parent) {
//             this.uiModal.node.removeFromParent();
//         }
//         for(let i=coms.length-1; i>=0; i--) {
//             if(coms[i].modalType.opacity > 0) {
//                 cc.find(ModalMgr.popUpRoot).addChild(this.uiModal.node, Math.max(coms[i].node.zIndex-1, 0));
//                 this.uiModal.fid = coms[i].fid;
//                 this.showModal(coms[i].modalType);
//                 break;
//             }
//         }   
//     }
// }
