import { _decorator, UIRenderer, Enum } from 'cc';
const {ccclass, property} = _decorator;

//import BatchAssembler from "../Assemblers/BatchAssembler";
enum TraversalMode {
    Default,
    SameName,
}

@ccclass('BatchComponent')
export default class BantchComponent extends UIRenderer {
    @property({type: Enum(TraversalMode), tooltip: "遍历模式: Default 默认的广度遍历, SameName 同名结点同批次"})
    mode: TraversalMode = 0;
    onEnable() {
        // super.onEnable();
        // if (!CC_NATIVERENDERER) this.node._renderFlag |= cc.RenderFlow.FLAG_POST_RENDER;
    }
    _resetAssembler() {
        // this.setVertsDirty();
        // let assembler = this._assembler = new BatchAssembler();
        // assembler.init(this);
    }
}


/**
 * 注意：已把原脚本注释，由于脚本变动过大，转换的时候可能有遗落，需要自行手动转换
 */
// import BatchAssembler from "../Assemblers/BatchAssembler";
// 
// enum TraversalMode {
//     Default,
//     SameName,
// 
// }
// 
// const {ccclass, property} = cc._decorator;
// 
// @ccclass
// export default class BantchComponent extends cc.RenderComponent {
// 
//     @property({type: cc.Enum(TraversalMode), tooltip: "遍历模式: Default 默认的广度遍历, SameName 同名结点同批次"})
//     mode: TraversalMode = 0;
// 
//     onEnable() {
//         super.onEnable();
//         if (!CC_NATIVERENDERER) this.node._renderFlag |= cc.RenderFlow.FLAG_POST_RENDER;    
//     }
// 
//     _resetAssembler() {
//         this.setVertsDirty();
//         let assembler = this._assembler = new BatchAssembler();
//         assembler.init(this);
//     }
// }
