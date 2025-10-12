import { _decorator } from 'cc';
const {ccclass, property} = _decorator;

import AdapterMgr, { AdapterType } from "../UIFrame/AdapterMgr";
import { UIFixed } from "../UIFrame/UIForm";

@ccclass('UISound')
export default class UISound extends UIFixed {
//    // LIFE-CYCLE CALLBACKS:
//    // onLoad () {}
    start () {
        AdapterMgr.inst.adapteByType(AdapterType.Right | AdapterType.Top, this.node);
    }
//    // update (dt) {}
}


/**
 * 注意：已把原脚本注释，由于脚本变动过大，转换的时候可能有遗落，需要自行手动转换
 */
// import AdapterMgr, { AdapterType } from "../UIFrame/AdapterMgr";
// import { UIFixed } from "../UIFrame/UIForm";
// 
// const {ccclass, property} = cc._decorator;
// 
// @ccclass
// export default class UISound extends UIFixed {
// 
// 
//     // LIFE-CYCLE CALLBACKS:
// 
//     // onLoad () {}
// 
//     start () {
//         
//         AdapterMgr.inst.adapteByType(AdapterType.Right | AdapterType.Top, this.node);
//     }
// 
//     // update (dt) {}
// }
