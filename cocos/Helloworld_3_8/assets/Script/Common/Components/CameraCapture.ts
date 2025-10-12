import { _decorator, Component, Node, Camera } from 'cc';
const {ccclass, property} = _decorator;

import { CommonUtils } from "../Utils/CommonUtils";

@ccclass('CameraCapture')
export default class CameraCapture extends Component {
    static inst: CameraCapture = null;
    @property(Node)
    captureNode: Node | null = null;
    private camera: Camera | null = null;
    onLoad () {
        // CameraCapture.inst = this;
        // this.camera = this.getComponent(cc.Camera);
        // if(!this.camera) {
        // this.camera = this.addComponent(cc.Camera);
        // }
        // this.node.active = false;

    }
    start () {}
    captureTexture() {
        // this.node.active = true;
        // this.captureNode.active = false;
        // let data = CocosHelper.captureScreen(this.camera, this.captureNode);
        // this.captureNode.active = true;
        // this.node.active = false;

        // return data;
    }
}


/**
 * 注意：已把原脚本注释，由于脚本变动过大，转换的时候可能有遗落，需要自行手动转换
 */
// import CocosHelper from "../../UIFrame/CocosHelper";
// import { CommonUtils } from "../Utils/CommonUtils";
// 
// const {ccclass, property} = cc._decorator;
// 
// @ccclass
// export default class CameraCapture extends cc.Component {
// 
//     static inst: CameraCapture = null;
// 
//     @property(cc.Node)
//     captureNode: cc.Node = null;
// 
//     private camera: cc.Camera = null;
//     onLoad () {
//         CameraCapture.inst = this;
//         this.camera = this.getComponent(cc.Camera);
//         if(!this.camera) {
//             this.camera = this.addComponent(cc.Camera);
//         }
//         this.node.active = false;
// 
//     }
// 
//     start () {}
// 
//     captureTexture() {
//         this.node.active = true;
//         this.captureNode.active = false;
//         let data = CocosHelper.captureScreen(this.camera, this.captureNode);
//         this.captureNode.active = true;
//         this.node.active = false;
//         
//         return data;
//     }
// 
// 
// }
