import { _decorator } from 'cc';
import { GameMain } from "../GameMain"

export class BaseMgr {
    game: GameMain = null;
    constructor(gameMain: GameMain) {
        this.game = gameMain;
    }
}


/**
 * 注意：已把原脚本注释，由于脚本变动过大，转换的时候可能有遗落，需要自行手动转换
 */
// import { Game } from "../Game"
// export class BaseMgr {
//     game: Game = null;
//     constructor(game: Game) {
//         this.game = game;
//     }
// }
