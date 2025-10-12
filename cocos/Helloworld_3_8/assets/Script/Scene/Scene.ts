import { _decorator, Component, Node,BlockInputEvents,warn,sys,game,Game,director } from 'cc';
const {ccclass, property} = _decorator;

import AdapterMgr, { AdapterType } from "../UIFrame/AdapterMgr";
import { EventCenter } from "../UIFrame/EventCenter";
import { EventType } from "../UIFrame/EventType";
import GameMgr from '../Logic/GameMain';

@ccclass('Scene')
export default class Scene extends Component {
    public static inst: Scene = null;
    private ndBlock: Node | null = null;
    onLoad() {
        //this.initBlockNode();
    }

    public initBlockNode() {
        // 启用事件阻断  会报错!!! todo
        this.ndBlock = new Node("block");
        const blockComp = this.ndBlock.getComponent(BlockInputEvents) || this.ndBlock.addComponent(BlockInputEvents);
        //this.node.addChild(this.ndBlock, cc.macro.MAX_ZINDEX);
        blockComp.enabled = true;
        this.ndBlock.active = true;

        this.node.setSiblingIndex(Number.MAX_SAFE_INTEGER);
        this.node.addChild(this.ndBlock);
    }
    public async start() {
         Scene.inst = this;
         AdapterMgr.inst.adapteByType(AdapterType.StretchHeight | AdapterType.StretchWidth, this.node);
         await this.onGameInit();
         this.registerEvent();
    }
//    /** 游戏初始化 */
    public async onGameInit() {
//        // 第一步 展示loading页面，当然有些默认就是loading页面

//        // 第二步 初始化游戏（Managers，Configs，SDKs）
         await GameMgr.init(this.node);
//        // 第三步 构建初始场景（加载必要的prefab，音频，texture）

//        // 第四步 加载主界面UI,关掉loading页面,正式进入游戏

    }

//    /** 初始化事件 */
    private registerEvent() {
         if (sys.platform === sys.Platform.WECHAT_GAME) {
            // 微信小游戏平台
            window.wx.onShow(this.onGameShow.bind(this));
            window.wx.onHide(this.onGameHide.bind(this));
        } else {
            // 其他平台
            game.on(Game.EVENT_SHOW, this.onGameShow, this);
            game.on(Game.EVENT_HIDE, this.onGameHide, this);
        }
    }
    private onGameShow() {
        EventCenter.emit(EventType.GameShow);
        director.resume()
    }
    private onGameHide() {
        EventCenter.emit(EventType.GameHide);
        director.pause();
    }
    public update(dt: number) {
        GameMgr.update(dt);
    }
    public lateUpdate() {

    }
//    /** 设置是否阻挡游戏触摸输入 */
    private _block = 0;
    public setInputBlock(bool: boolean) {
         if(!this.ndBlock) {
         warn("未启用 block input");
         return ;
         }
         bool ? ++ this._block : -- this._block;
         this.ndBlock.active = this._block > 0;
    }
}


/**
 * 注意：已把原脚本注释，由于脚本变动过大，转换的时候可能有遗落，需要自行手动转换
 */
// import Game from "../Logic/Game";
// import AdapterMgr, { AdapterType } from "../UIFrame/AdapterMgr";
// import { EventCenter } from "../UIFrame/EventCenter";
// import { EventType } from "../UIFrame/EventType";
// 
// const {ccclass, property} = cc._decorator;
// 
// @ccclass
// export default class Scene extends cc.Component {
// 
//     public static inst: Scene = null;
//     private ndBlock: cc.Node = null;
//     onLoad() {
//         this.initBlockNode();
//     }
// 
//     public initBlockNode() {
//         this.ndBlock = new cc.Node("block");
//         this.ndBlock.addComponent(cc.BlockInputEvents);
//         this.node.addChild(this.ndBlock, cc.macro.MAX_ZINDEX);
//     }
// 
//     public async start() {
//         Scene.inst = this;
//         AdapterMgr.inst.adapteByType(AdapterType.StretchHeight | AdapterType.StretchWidth, this.node);
//         await this.onGameInit();
//         this.registerEvent();
//     }
//     /** 游戏初始化 */
//     public async onGameInit() {
//         // 第一步 展示loading页面，当然有些默认就是loading页面
// 
//         // 第二步 初始化游戏（Managers，Configs，SDKs）
//         await Game.init(this.node);
//         // 第三步 构建初始场景（加载必要的prefab，音频，texture）
// 
//         // 第四步 加载主界面UI,关掉loading页面,正式进入游戏
// 
//     }
//     /** 初始化事件 */
//     private registerEvent() {
//         if(cc.sys.platform === cc.sys.WECHAT_GAME) {
//             wx.onShow(this.onGameShow.bind(this));
//             wx.onHide(this.onGameHide.bind(this));
//         }else {
//             cc.game.on(cc.game.EVENT_SHOW, this.onGameShow, this);
//             cc.game.on(cc.game.EVENT_HIDE, this.onGameHide, this);
//         }
//     }
// 
//     private onGameShow(param: any) {
//         EventCenter.emit(EventType.GameShow, param);
//         cc.director.resume()
//     }
//     private onGameHide() {
//         EventCenter.emit(EventType.GameHide);
//         cc.director.pause();
//     }
// 
//     public update(dt: number) {
//         Game.update(dt);
//     }
// 
//     public lateUpdate() {
// 
//     }
// 
//     /** 设置是否阻挡游戏触摸输入 */
//     private _block = 0;
//     public setInputBlock(bool: boolean) {
//         if(!this.ndBlock) {
//             cc.warn("未启用 block input");
//             return ;
//         }
//         bool ? ++ this._block : -- this._block;
//         this.ndBlock.active = this._block > 0;
//     }
// }
