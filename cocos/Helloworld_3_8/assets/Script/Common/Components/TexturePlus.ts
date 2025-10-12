//import { _decorator, renderer, Vec2, Mat4, UIRenderer, Texture2D, Enum, macro, CCBoolean, Assembler } from 'cc';
//const {ccclass, menu, executeInEditMode, mixins, property} = _decorator;
//
//import TextureAssembler from "../Assemblers/TextureAssembler";
//import { CommonUtils } from "../Utils/CommonUtils";
//const renderEngine = renderer.renderEngine;
//enum TextureType {
//    Cut,            // 裁剪
//    Stretch         // 拉伸, 暂未实现
//}
//let _vec2_temp = new Vec2();
//let _mat4_temp = new Mat4();
//
//@ccclass('TexturePlus')
//@executeInEditMode@menu('i18n:MAIN_MENU.component.ui/TexturePlus')@mixins(cc.BlendFunc)
//export default class TexturePlus extends UIRenderer {
//    static Type = TextureType;
//    
//    @property(Texture2D)
//    @property(Texture2D)
//    get texture() {
//        // return this._texture;
//    }
//    set texture(val: Texture2D) {
//        // this._texture = val;
//        // let l = -val.width/2, b = -val.height/2, t = val.height/2, r = val.width/2;
//        // this.polygon = [cc.v2(l, b), cc.v2(r, b), cc.v2(r, t), cc.v2(l, t)];
//        // this.node.width = val.width; this.node.height = val.height;
//        // this._updateMaterial();
//    }
////    // _type: TextureType = 0;
////    // @property({type: cc.Enum(TextureType), serializable: true})
////    // get type() {
////    //     return this._type;
////    // }
////    // set type(val: TextureType) {
////    //     this._type = val;
////    //     this.setVertsDirty();
////    // }
//    @property({type: [Vec2], serializable: true})
//    _polygon: Vec2[] = [];
//    @property({type: [Vec2], serializable: true})
//    public get polygon() {
//        // return this._polygon;
//    }
//    public set polygon(points: Vec2[]) {
//        // this._polygon = points;
//        // this._updateVerts();
//    }
//    @property({type: cc.Enum(macro.BlendFactor), override: true})
//    srcBlendFactor: macro.BlendFactor = macro.BlendFactor.SRC_ALPHA;
//    @property({type: cc.Enum(macro.BlendFactor), override: true})
//    dstBlendFactor: macro.BlendFactor = macro.BlendFactor.ONE_MINUS_SRC_ALPHA;
//    @property(CCBoolean)
//    editing: boolean = false;
//    
//    onLoad() {
//        // this.node['_hitTest'] = this._hitTest.bind(this);
//    }
//    start() {
//        // this.node.on(cc.Node.EventType.TOUCH_START, () => {
//        // console.log("click texture plus");
//        // }, this);
//        // this.node.on(cc.Node.EventType.TOUCH_MOVE, (e: cc.Event.EventTouch) => {
//        // this.node.x += e.getDeltaX();
//        // this.node.y += e.getDeltaY();
//        // }, this);
//    }
//    private _updateVerts() {
//        // this.setVertsDirty();
//    }
//    public _updateMaterial() {
//        // let texture = this._texture;
//        // let material = this.getMaterial(0);
//        // if(material) {
//        // if(material.getDefine("USE_TEXTURE") !== undefined) {
//        // material.define("USE_TEXTURE", true);
//        // }
//        // material.setProperty("texture", texture);
//        // }
//        // this['__proto__']._updateBlendFunc.call(this);
//        // this.setVertsDirty();
//    }
//    public _validateRender() {
//
//    }
//    public _resetAssembler() {
//        // let assembler = this._assembler = new TextureAssembler();
//        // assembler.init(this);
//        // this._updateColor();
//        // this.setVertsDirty();
//    }
//    _hitTest (cameraPt: Vec2) {
//        // let node = this.node;
//        // let size = node.getContentSize(),
//        // w = size.width,
//        // h = size.height,
//        // testPt = _vec2_temp;
//
//        // node['_updateWorldMatrix']();
////        // If scale is 0, it can't be hit.
//        // if (!cc.Mat4.invert(_mat4_temp, node['_worldMatrix'])) {
//        // return false;
//        // }
//        // cc.Vec2.transformMat4(testPt, cameraPt, _mat4_temp);
//        // return CommonUtils.isInPolygon(testPt, this.polygon);
//    }
//}


/**
 * 注意：已把原脚本注释，由于脚本变动过大，转换的时候可能有遗落，需要自行手动转换
 */
// import TextureAssembler from "../Assemblers/TextureAssembler";
// import { CommonUtils } from "../Utils/CommonUtils";
// 
// const renderEngine = cc.renderer.renderEngine;
// 
// enum TextureType {
//     Cut,            // 裁剪
//     Stretch         // 拉伸, 暂未实现
// }
// 
// let _vec2_temp = new cc.Vec2();
// let _mat4_temp = new cc.Mat4();
// 
// const {ccclass, menu, executeInEditMode, mixins, property} = cc._decorator;
// 
// @ccclass
// @executeInEditMode
// @menu('i18n:MAIN_MENU.component.ui/TexturePlus')
// @mixins(cc.BlendFunc)
// export default class TexturePlus extends cc.RenderComponent {
//     static Type = TextureType;
//     
//     @property(cc.Texture2D)
//     _texture: cc.Texture2D = null;
//     @property(cc.Texture2D)
//     get texture() {
//         return this._texture;
//     }
//     set texture(val: cc.Texture2D) {
//         this._texture = val;
//         let l = -val.width/2, b = -val.height/2, t = val.height/2, r = val.width/2;
//         this.polygon = [cc.v2(l, b), cc.v2(r, b), cc.v2(r, t), cc.v2(l, t)];
//         this.node.width = val.width; this.node.height = val.height;
//         this._updateMaterial();
//     }
// 
//     // _type: TextureType = 0;
//     // @property({type: cc.Enum(TextureType), serializable: true})
//     // get type() {
//     //     return this._type;
//     // }
//     // set type(val: TextureType) {
//     //     this._type = val;
//     //     this.setVertsDirty();
//     // }
// 
//     @property({type: [cc.Vec2], serializable: true})
//     _polygon: cc.Vec2[] = [];
//     @property({type: [cc.Vec2], serializable: true})
//     public get polygon() {
//         return this._polygon;
//     }
//     public set polygon(points: cc.Vec2[]) {
//         this._polygon = points;
//         this._updateVerts();
//     }
// 
//     @property({type: cc.Enum(cc.macro.BlendFactor), override: true})
//     srcBlendFactor: cc.macro.BlendFactor = cc.macro.BlendFactor.SRC_ALPHA;
// 
//     @property({type: cc.Enum(cc.macro.BlendFactor), override: true})
//     dstBlendFactor: cc.macro.BlendFactor = cc.macro.BlendFactor.ONE_MINUS_SRC_ALPHA;
// 
//     @property(cc.Boolean)
//     editing: boolean = false;
//     
//     _assembler: cc.Assembler = null;
// 
//     onLoad() {
//         this.node['_hitTest'] = this._hitTest.bind(this);
//     }
// 
//     start() {
//         this.node.on(cc.Node.EventType.TOUCH_START, () => {
//             console.log("click texture plus");
//         }, this);
//         this.node.on(cc.Node.EventType.TOUCH_MOVE, (e: cc.Event.EventTouch) => {
//             this.node.x += e.getDeltaX();
//             this.node.y += e.getDeltaY();
//         }, this);
//     }
// 
//     private _updateVerts() {
//         this.setVertsDirty();
//     }
// 
//     public _updateMaterial() {
//         let texture = this._texture;
//         let material = this.getMaterial(0);
//         if(material) {
//             if(material.getDefine("USE_TEXTURE") !== undefined) {
//                 material.define("USE_TEXTURE", true);
//             }
//             material.setProperty("texture", texture);
//         }
//         this['__proto__']._updateBlendFunc.call(this);
//         this.setVertsDirty();        
//     }
// 
//     public _validateRender() {
//         
//     }
// 
//     public _resetAssembler() {
//         let assembler = this._assembler = new TextureAssembler();
//         assembler.init(this);
//         this._updateColor();
//         this.setVertsDirty();
//     }
// 
//     _hitTest (cameraPt: cc.Vec2) {
//         let node = this.node;
//         let size = node.getContentSize(),
//             w = size.width,
//             h = size.height,
//             testPt = _vec2_temp;
//         
//         node['_updateWorldMatrix']();
//         // If scale is 0, it can't be hit.
//         if (!cc.Mat4.invert(_mat4_temp, node['_worldMatrix'])) {
//             return false;
//         }
//         cc.Vec2.transformMat4(testPt, cameraPt, _mat4_temp);
//         return CommonUtils.isInPolygon(testPt, this.polygon);
//     }
// }
