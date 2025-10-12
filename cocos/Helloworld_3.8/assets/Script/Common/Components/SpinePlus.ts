//import { _decorator } from 'cc';
//const {ccclass, property, executeInEditMode, menu, help, inspector} = _decorator;
//
//@ccclass('SpinePlus')
//@menu('i18n:MAIN_MENU.component.ui/SpinePlus')@executeInEditMode@help('app://docs/html/components/spine.html')@inspector('packages://custom-inspector/inspector/spine.js')
//export default class SpinePlus extends sp.Skeleton {
//    @property({override: true, visible: true})
//    paused = false;
////    // onLoad () {}
//    start () {
//
//    }
//    update (dt: number) {
//        // if(!CC_EDITOR) {
//        // super.update(dt);
//        // return ;
//        // }
//        // if (this.paused) return;
////        //@ts-ignore
//        // cc.engine._animatingInEditMode = 1;
////        //@ts-ignore
//        // cc.engine.animatingInEditMode = 1;
////        //@ts-ignore
//        // dt *= this.timeScale * sp.timeScale;
////        // @ts-ignore
//        // this._updateRealtime(dt);
//    }
//}


/**
 * 注意：已把原脚本注释，由于脚本变动过大，转换的时候可能有遗落，需要自行手动转换
 */
// const {ccclass, property, executeInEditMode, menu, help, inspector} = cc._decorator;
// @ccclass
// @menu('i18n:MAIN_MENU.component.ui/SpinePlus')
// @executeInEditMode
// @help('app://docs/html/components/spine.html')
// @inspector('packages://custom-inspector/inspector/spine.js')
// export default class SpinePlus extends sp.Skeleton {
// 
//     @property({override: true, visible: true})
//     paused = false;
// 
// 
//     // onLoad () {}
// 
//     start () {
//         
//     }
// 
//     update (dt: number) {
//         if(!CC_EDITOR) {
//             super.update(dt);
//             return ;
//         }
//         if (this.paused) return;
//         //@ts-ignore
//         cc.engine._animatingInEditMode = 1;
//         //@ts-ignore
//         cc.engine.animatingInEditMode = 1;
//         //@ts-ignore
//         dt *= this.timeScale * sp.timeScale;
//         // @ts-ignore
//         this._updateRealtime(dt);
//     }
// }
