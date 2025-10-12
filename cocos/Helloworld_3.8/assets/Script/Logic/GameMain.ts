// /**
//  * 掌管逻辑层
//  */

import { _decorator, Node } from 'cc';
import ConfigMgr from "./Manager/ConfigMgr";
import PlayerMgr from "./Manager/PlayerMgr";


export class GameMain {
    public inited = false;
    public configMgr: ConfigMgr = null;
    public playerMgr: PlayerMgr = null;
    public async init(uiRoot: Node) {
//        // 初始化Manager, 例如new ConfigMgr();
        this.configMgr = new ConfigMgr(this);
        // this.playerMgr = new PlayerMgr(this);
//        // 初始化平台sdk
//        // todo...
//        // 加载配置
        await this.configMgr.loadConfigs();

//        // 
        this.inited = true;
    }
    onGameShow() {

    }
    onGameHide() {

    }
//    /**
//     * 逻辑层的时间更新控制
//     * @param dt 
//     */
    public update(dt: number) {
        // if(!this.inited) return ;
//        // 例如Task.update(dt); 更新任务进度
    }
}

let GameMgr = new GameMain();
export default GameMgr;

/**
 * 注意：已把原脚本注释，由于脚本变动过大，转换的时候可能有遗落，需要自行手动转换
 */
// import ConfigMgr from "./Manager/ConfigMgr";
// import PlayerMgr from "./Manager/PlayerMgr";
// 
// /**
//  * 掌管逻辑层
//  */
// export class Game {
// 
//     public inited = false;
//     public configMgr: ConfigMgr = null;
//     public playerMgr: PlayerMgr = null;
//     public async init(uiRoot: cc.Node) {
//         // 初始化Manager, 例如new ConfigMgr();
//         this.configMgr = new ConfigMgr(this);
//         this.playerMgr = new PlayerMgr(this);
//         // 初始化平台sdk
//         // todo...
//         // 加载配置
//         await this.configMgr.loadConfigs();
// 
//         // 
//         this.inited = true;
//     }
// 
//     onGameShow() {
// 
//     }
//     onGameHide() {
//         
//     }
// 
//     /**
//      * 逻辑层的时间更新控制
//      * @param dt 
//      */
//     public update(dt: number) {
//         if(!this.inited) return ;
//         // 例如Task.update(dt); 更新任务进度
//     }
// }
// let GameMgr = new Game();
// export default GameMgr;
