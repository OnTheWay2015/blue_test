
/*
import { _decorator, Enum, Component } from 'cc';
const {ccclass, executeInEditMode, menu, property} = _decorator;

export enum PropEmum {
    All,
    Active,
    Position,
    Rotation,
    Scale,
    Anchor,
    Size,
    Color,
    Opacity,
    Slew,
    Label_String,
    Sprite_Texture,
}
cc['PropEmum'] = PropEmum;
const ControllerId = Enum({});

@ccclass('PropSelector')
@menu('i18n:状态控制/PropSelector')@executeInEditMode
export default class PropSelector extends Component {
    
//    // @property({type: ControllerId, tooltip: "控制器的名称"})
//    // ctrlId = 0;
    
//    // 被控制的属性
    @property({type: [Enum(PropEmum)], tooltip: "被控制的属性"})               
    props: PropEmum[] = [0];    
//    // update (dt) {} 
}

*/


/**
 * 注意：已把原脚本注释，由于脚本变动过大，转换的时候可能有遗落，需要自行手动转换
 */
// const {ccclass, executeInEditMode, menu, property} = cc._decorator;
// 
// export enum PropEmum {
//     All,
//     Active,
//     Position,
//     Rotation,
//     Scale,
//     Anchor,
//     Size,
//     Color,
//     Opacity,
//     Slew,
// 
//     Label_String,
//     Sprite_Texture,
// }
// 
// cc['PropEmum'] = PropEmum;
// 
// const ControllerId = cc.Enum({});
// 
// @ccclass
// @menu('i18n:状态控制/PropSelector')
// @executeInEditMode
// export default class PropSelector extends cc.Component {
//     
//     // @property({type: ControllerId, tooltip: "控制器的名称"})
//     // ctrlId = 0;
//     
//     // 被控制的属性
//     @property({type: [cc.Enum(PropEmum)], tooltip: "被控制的属性"})               
//     props: PropEmum[] = [0];    
// 
//     // update (dt) {} 
// }
