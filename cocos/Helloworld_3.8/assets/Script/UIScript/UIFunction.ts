import { _decorator } from 'cc';
const {ccclass, property} = _decorator;

import UIFunction_Auto from "../AutoScripts/UIFunction_Auto";
import AdapterMgr, { AdapterType } from "../UIFrame/AdapterMgr";
import FormMgr from "../UIFrame/FormMgr";
import { UIFixed } from "../UIFrame/UIForm";
import WindowMgr from "../UIFrame/WindowMgr";
import UISkills from "./UISkills";

@ccclass('UIFunction')
export default class UIFunction extends UIFixed {
    
    view: UIFunction_Auto;
//    // onLoad () {}
    start () {
        // AdapterMgr.inst.adapteByType(AdapterType.Bottom, this.node, 50);

        // this.view.Setting.addClick(() => {
        // FormMgr.open(UIConfig.UISetting);
        // }, this);

        // this.view.Skills.addClick(() => {
        // FormMgr.open(UIConfig.UISkills);
        // }, this);

        // this.view.ScrollHelper.addClick(() => {
        // FormMgr.open(UIConfig.UIScrollPlus)
        // }, this);


    }
//    // update (dt) {}
}


/**
 * 注意：已把原脚本注释，由于脚本变动过大，转换的时候可能有遗落，需要自行手动转换
 */
// import UIFunction_Auto from "../AutoScripts/UIFunction_Auto";
// import AdapterMgr, { AdapterType } from "../UIFrame/AdapterMgr";
// import FormMgr from "../UIFrame/FormMgr";
// import { UIFixed } from "../UIFrame/UIForm";
// import WindowMgr from "../UIFrame/WindowMgr";
// import UIConfig from "./../UIConfig";
// import UISkills from "./UISkills";
// 
// const {ccclass, property} = cc._decorator;
// 
// @ccclass
// export default class UIFunction extends UIFixed {
//     
//     view: UIFunction_Auto;
//     // onLoad () {}
// 
//     start () {
//         AdapterMgr.inst.adapteByType(AdapterType.Bottom, this.node, 50);
//         
//         this.view.Setting.addClick(() => {            
//             FormMgr.open(UIConfig.UISetting);
//         }, this);
// 
//         this.view.Skills.addClick(() => {
//             FormMgr.open(UIConfig.UISkills);
//         }, this);
// 
//         this.view.ScrollHelper.addClick(() => {
//             FormMgr.open(UIConfig.UIScrollPlus)
//         }, this);
// 
//         
//     }
// 
//     // update (dt) {}
// }
