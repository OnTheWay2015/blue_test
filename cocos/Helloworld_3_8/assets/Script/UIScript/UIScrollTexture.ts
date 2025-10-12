import { _decorator, Sprite } from 'cc';
const {ccclass, property} = _decorator;

import { UIScreen } from "../UIFrame/UIForm";

@ccclass('UIScrollTexture')
export default class UIScrollTexture extends UIScreen {
//    // onLoad () {}
    start () {

    }
    private turn = 1;
    private progress = 0;
    update (dt) {
        // this.progress += dt * this.turn * 0.2;
        // this.spTexture.getMaterial(0).setProperty('progress', this.progress);
        // if(this.progress >= 1) {
        // this.turn = -1;
        // }
        // if(this.progress <= 0) {
        // this.turn = 1;
        // }
    }
}


/**
 * 注意：已把原脚本注释，由于脚本变动过大，转换的时候可能有遗落，需要自行手动转换
 */
// import { UIScreen } from "../UIFrame/UIForm";
// 
// const {ccclass, property} = cc._decorator;
// 
// @ccclass
// export default class UIScrollTexture extends UIScreen {
// 
//     @property(cc.Sprite) spTexture: cc.Sprite = null;
// 
// 
//     // onLoad () {}
// 
//     start () {
// 
//     }
// 
//     private turn = 1;
//     private progress = 0;
//     update (dt) {
//         this.progress += dt * this.turn * 0.2;
//         this.spTexture.getMaterial(0).setProperty('progress', this.progress);
//         if(this.progress >= 1) {
//             this.turn = -1;
//         }
//         if(this.progress <= 0) {
//             this.turn = 1;
//         }
//     }
// }
