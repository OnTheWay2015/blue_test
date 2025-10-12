import { _decorator, easing, tween } from 'cc';
const {ccclass, property} = _decorator;

import UILoading_Auto from "../AutoScripts/UILoading_Auto";
import AdapterMgr from "../UIFrame/AdapterMgr";
import { UITips } from "../UIFrame/UIForm";
import CocosHelper from '../Common/Utils/CocosHelper';

@ccclass('UILoading')
export default class UILoading extends UITips {
    
    view: UILoading_Auto;//类名和编辑器组件名要一样 可以自动绑定
    onLoad () {

    }
    start () {
        console.log("UILoading start!");
    }
    async showEffect() {
        let len = AdapterMgr.inst.visibleSize.width/2 +  CocosHelper.NodeGetWidth(this.view.Left)/2;
        this.view.Left.x = -len;
        this.view.Right.x = len;

        //{ easing: Easing.QuadInOut }
        await Promise.all([
        CocosHelper.runTweenSync(this.view.Left, tween().to(0.3, {x: -len}, {easing:easing.quadInOut} )),
        CocosHelper.runTweenSync(this.view.Right, tween().to(0.3, {x: len}, {easing:easing.quadInOut} ))
        ]);
    }
    async hideEffect() {
        let len = AdapterMgr.inst.visibleSize.width/2 +  CocosHelper.NodeGetWidth(this.view.Left)/2;
        this.view.Left.x = -228;
        this.view.Right.x = 228;

         await CocosHelper.sleepSync(0.5);
         await Promise.all([
        // CocosHelper.runTweenSync(this.view.Left, cc.tween().to(0.3, {x: -len}, cc.easeIn(3.0))),
        CocosHelper.runTweenSync(this.view.Left, tween().to(0.3, {x: -len}, {easing:easing.quadInOut} )),
        CocosHelper.runTweenSync(this.view.Right, tween().to(0.3, {x: len}, {easing:easing.quadInOut} )),
         ]);
    }
    //update (dt) {}
}


/**
 * 注意：已把原脚本注释，由于脚本变动过大，转换的时候可能有遗落，需要自行手动转换
 */
// import UILoading_Auto from "../AutoScripts/UILoading_Auto";
// import AdapterMgr from "../UIFrame/AdapterMgr";
// import CocosHelper from "../UIFrame/CocosHelper";
// import { UITips } from "../UIFrame/UIForm";
// 
// const {ccclass, property} = cc._decorator;
// 
// @ccclass
// export default class UILoading extends UITips {
//     
//     view: UILoading_Auto;
// 
//     // onLoad () {}
// 
//     start () {
// 
//     }
// 
//     async showEffect() {
//         let len = AdapterMgr.inst.visibleSize.width/2 + this.view.Left.width/2;
//         this.view.Left.x = -len;
//         this.view.Right.x = len;
// 
//         await Promise.all([
//             CocosHelper.runTweenSync(this.view.Left, cc.tween().to(0.3, {x: -228}, cc.easeIn(3.0))),
//             CocosHelper.runTweenSync(this.view.Right, cc.tween().to(0.3, {x: 228}, cc.easeIn(3.0)))
//         ]);
//     }
// 
//     async hideEffect() {
//         let len = AdapterMgr.inst.visibleSize.width/2 + this.view.Left.width/2;
//         this.view.Left.x = -228;
//         this.view.Right.x = 228;
// 
//         await CocosHelper.sleepSync(0.5);
//         await Promise.all([
//             CocosHelper.runTweenSync(this.view.Left, cc.tween().to(0.3, {x: -len}, cc.easeIn(3.0))),
//             CocosHelper.runTweenSync(this.view.Right, cc.tween().to(0.3, {x: len}, cc.easeIn(3.0)))
//         ]);
//     }
// 
//     // update (dt) {}
// }
