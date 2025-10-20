import { _decorator, Asset, assetManager, AssetManager, Component, error, EventTouch, ImageAsset, instantiate, JsonAsset, Node, Prefab, resources, SceneAsset, Sprite, SpriteFrame, Texture2D, UITransform, Vec3 } from 'cc';
const { ccclass, property } = _decorator;


@ccclass('TestDrag')
export class TestDrag extends Component {
    //private boundaryNode: cc.Node = null; // 拖拽边界节点

    private _offset: Vec3 = new Vec3();
    private _isDragging: boolean = false;

    protected onLoad(): void {
        this.node.on(Node.EventType.TOUCH_START, this.onTouchStart, this);
        this.node.on(Node.EventType.TOUCH_MOVE, this.onTouchMove, this);
        this.node.on(Node.EventType.TOUCH_END, this.onTouchEnd, this);
        //this.node.on(Node.EventType.TOUCH_CANCEL, this.onTouchEnd, this);
        
    
        const trans = this.node.getComponent(UITransform)
        console.log(`注意鼠标事件响应范围: width[${trans?.width}] height[${trans?.height}]`)
    
    }
    private onTouchEnd() {
        this._isDragging = false;
    }
   
    private onTouchStart(event: EventTouch) {
        const worldPos = event.getUILocation();
        const localPos = this.node.parent!.getComponent(UITransform)!
            .convertToNodeSpaceAR(new Vec3(worldPos.x, worldPos.y));
            
        this._isDragging = true;
        this._offset.set(this.node.position.x - localPos.x, this.node.position.y - localPos.y);
    }

    private onTouchMove(event: EventTouch) {
        if (!this._isDragging){
            return;
        }
        const worldPos = event.getUILocation();
        let newPos = this.node.parent!.getComponent(UITransform)!
            .convertToNodeSpaceAR(new Vec3(worldPos.x, worldPos.y));
        
        newPos.add(this._offset);

        //if (this.constrainToCanvas) {
        //    newPos.x = clamp(newPos.x, this._canvasRect.xMin, this._canvasRect.xMax);
        //    newPos.y = clamp(newPos.y, this._canvasRect.yMin, this._canvasRect.yMax);
        //}
        
        this.node.setPosition(newPos);
    }

    //public static addSimpleClick(target: Node, cb: () => void) {
    //    let targetNode = target;
    //    let lastTouchPos: Vec2 = null;
    //    targetNode.on(Node.EventType.TOUCH_START, (e: EventTouch) => {
    //        lastTouchPos = e.getLocation();
    //    }, this);
    //    targetNode.on(Node.EventType.TOUCH_END, (e: EventTouch) => {
    //        if (lastTouchPos) {
    //             
    //            const delta = lastTouchPos.subtract(e.getLocation()).length();
    //           
    //            if (delta < 3) {
    //                cb();
    //            }
    //        }
    //    }, this);
    //}
/*	
    private onTouchStart(event: EventTouch) {
        const collider = this.node.getComponent(cc.PolygonCollider2D);
        if (collider) {
            const touchPos = event.getLocation();
            const localPos = this.node.getComponent(cc.UITransform).convertToNodeSpaceAR(cc.v3(touchPos.x, touchPos.y));
            if (!Intersection2D.pointInPolygon(localPos, collider.points)) {
                return; // 点击在透明区域时不触发拖拽
            }
        }

        const worldPos = this.node.getWorldPosition();
        const touchWorldPos = cc.v3(event.getLocationX(), event.getLocationY());
        this._offset = worldPos.sub(touchWorldPos);
        this._isDragging = true;
        this.node.setSiblingIndex(999); // 拖拽时置顶
    }

    private onTouchMove(event: cc.EventTouch) {
        if (!this._isDragging) return;

        const newPos = cc.v3(
            event.getLocationX() + this._offset.x,
            event.getLocationY() + this._offset.y
        );

        if (this.boundaryNode) {
            const boundaryRect = this.boundaryNode.getBoundingBox();
            const nodeRect = this.node.getBoundingBox();
            newPos.x = cc.misc.clampf(
                newPos.x,
                boundaryRect.xMin + nodeRect.width / 2,
                boundaryRect.xMax - nodeRect.width / 2
            );
            newPos.y = cc.misc.clampf(
                newPos.y,
                boundaryRect.yMin + nodeRect.height / 2,
                boundaryRect.yMax - nodeRect.height / 2
            );
        }

        this.node.setPosition(newPos);
    }



    start() {
    }

    update(deltaTime: number) {
    }



*/

}
