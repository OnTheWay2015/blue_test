import { _decorator } from 'cc';
import { BaseMgr } from "./BaseMgr";

export default class ConfigMgr extends BaseMgr {
//    /** 加载配置文件 */
    async loadConfigs() {

    }
//    /** 通知其他manager */
    public onConfigChange() {
        this.game.playerMgr.onConfigChange()
    }
}


/**
 * 注意：已把原脚本注释，由于脚本变动过大，转换的时候可能有遗落，需要自行手动转换
 */
// import { BaseMgr } from "./BaseMgr";
// 
// export default class ConfigMgr extends BaseMgr {
// 
//     /** 加载配置文件 */
//     async loadConfigs() {
// 
//     }
// 
//     /** 通知其他manager */
//     public onConfigChange() {
//         this.game.playerMgr.onConfigChange()
//     }
// }
