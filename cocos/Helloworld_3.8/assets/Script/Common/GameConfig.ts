import { _decorator } from 'cc';
export default class GameConfig {
    public static gameId = '';
//    //游戏版本
    public static readonly version = '0.0.1';
    public static _debugUserId = '';
    public static get debugUserId() {
         return this._debugUserId;
    }
}


/**
 * 注意：已把原脚本注释，由于脚本变动过大，转换的时候可能有遗落，需要自行手动转换
 */
// export default class GameConfig {
// 
//     public static gameId = '';
//     //游戏版本
//     public static readonly version = '0.0.1';
// 
//     public static _debugUserId = '';
//     public static get debugUserId() {
//         return this._debugUserId;
//     }
// }
