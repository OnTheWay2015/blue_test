//import { _decorator, Component, PhysicsPolygonCollider } from 'cc';
import { _decorator, Component} from 'cc';
const {ccclass, property} = _decorator;

@ccclass('UIWave')
export default class UIWave extends Component {
    
//    // onLoad () {}
    start () {
        // this.polygonCollider.points = []
    }
//    // update (dt) {}
}


/**
 * 注意：已把原脚本注释，由于脚本变动过大，转换的时候可能有遗落，需要自行手动转换
 */
// const {ccclass, property} = cc._decorator;
// 
// @ccclass
// export default class UIWave extends cc.Component {
// 
//     @property(cc.PhysicsPolygonCollider) polygonCollider: cc.PhysicsPolygonCollider = null;
//     
// 
//     // onLoad () {}
// 
//     start () {
//         this.polygonCollider.points = []
//     }
// 
//     // update (dt) {}
// }
