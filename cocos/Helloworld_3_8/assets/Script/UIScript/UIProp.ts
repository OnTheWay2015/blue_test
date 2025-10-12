import { _decorator } from 'cc';
const {ccclass, property} = _decorator;

//import PropController from "../Common/Components/PropController";
import { UIScreen } from "../UIFrame/UIForm";

@ccclass('UIProp')
export default class UIProp extends UIScreen {
    //@property(PropController) building: PropController = null;
    //@property(PropController) building2: PropController = null;
    
//    // onLoad () {}
    start () {

    }
    onCtrl1(event, data) {
        // this.building.doControl(data);
    }
    onCtrl2(event, data) {
        // this.building2.doControl(data);
    }
//    // update (dt) {}
}


/**
 * 注意：已把原脚本注释，由于脚本变动过大，转换的时候可能有遗落，需要自行手动转换
 */
// import PropController from "../Common/Components/PropController";
// import { UIScreen } from "../UIFrame/UIForm";
// 
// const {ccclass, property} = cc._decorator;
// 
// @ccclass
// export default class UIProp extends UIScreen {
// 
//     @property(PropController) building: PropController = null;
//     @property(PropController) building2: PropController = null;
//     
//     // onLoad () {}
// 
//     start () {
// 
//     }
// 
//     onCtrl1(event, data) {
//         this.building.doControl(data);
//     }
// 
//     onCtrl2(event, data) {
//         this.building2.doControl(data);
//     }
// 
//     // update (dt) {}
// }
