import { _decorator, Component, Label } from 'cc';
const {ccclass, property} = _decorator;

import ResMgr from "../../UIFrame/ResMgr";

@ccclass('CacheUtils')
export default class CacheUtils extends Component {
//    // LIFE-CYCLE CALLBACKS:
    public com: Label | null = null;
    onLoad () {
        // this.com = this.getComponent(cc.Label);
    }
    start () {
        // this.node.on(cc.Node.EventType.TOUCH_MOVE, (e: cc.Event.EventTouch) => {
        // this.node.x += e.getDeltaX();
        // this.node.y += e.getDeltaY();
        // }, this);
    }
    private passTime = 0;
    update (dt) {
        // this.passTime += dt;
        // if(this.passTime > 1) {
        // this.passTime = 0;
        // this.com.string = ResMgr.inst.computeTextureCache();
        // }

    }
}


/**
 * 注意：已把原脚本注释，由于脚本变动过大，转换的时候可能有遗落，需要自行手动转换
 */
// import ResMgr from "../../UIFrame/ResMgr";
// 
// const {ccclass, property} = cc._decorator;
// 
// @ccclass
// export default class CacheUtils extends cc.Component {
// 
//     // LIFE-CYCLE CALLBACKS:
// 
//     public com: cc.Label = null;
//     onLoad () {
//         this.com = this.getComponent(cc.Label);
//     }
// 
//     start () {
//         this.node.on(cc.Node.EventType.TOUCH_MOVE, (e: cc.Event.EventTouch) => {
//             this.node.x += e.getDeltaX();
//             this.node.y += e.getDeltaY();
//         }, this);
//     }
// 
//     private passTime = 0;
//     update (dt) {
//         this.passTime += dt;
//         if(this.passTime > 1) {
//             this.passTime = 0;
//             this.com.string = ResMgr.inst.computeTextureCache();
//         }
//         
//     }
// }
