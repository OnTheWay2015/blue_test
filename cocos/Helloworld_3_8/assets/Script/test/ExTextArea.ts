import { _decorator, Color, Component, Node, RichText, ScrollBar, ScrollView, UITransform, Widget } from 'cc';
import CocosHelper from '../Common/Utils/CocosHelper';
const { ccclass, property } = _decorator;

@ccclass('ExTextArea')
export class ExTextArea extends Component {
@property(Node)
content:Node= null;


@property(ScrollView)
scroll:ScrollView= null;
//@property(RichText)
//richText:RichText = null;

//@property(cc.EditBox)
//editBox: cc.EditBox = null;

onLoad() {
    //this.editBox.multiline = true;
    //this.editBox.node.on('editing-did-ended', this.onTextSubmit, this);
}
    start() {
        for (let i=0;i<20;i++){
            this.addMsg(i.toString() + "_xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
        }
    }

    update(deltaTime: number) {

    }
    onTextSubmit() {
        //this.richText.string = this.editBox.string;
    }

    addMsg(msg:string){
        let node = new Node();
        node.x = 0;
        
        let t = node.addComponent(RichText);
        t.maxWidth = 300; 
        t.fontColor = Color.RED; 
        t.string = msg;
        
        let ts =t.getComponent(UITransform) || t.addComponent(UITransform)
        ts.anchorX = 0;
        ts.anchorY = 0;
        ts.width = 300;
        //node.y = 0;
        //let wt = t.addComponent(Widget)

        this.content.addChild(node);
        this.updateContentSize();

        this.scroll.scrollToBottom();
        //this.content.updateWorldTransform();
        //CocosHelper.TestTransProps(node);
    }

    // 更新content容器尺寸
    private updateContentSize() {
        if (!this.content) return;

        var totalHeight= 0;
        this.content.children.forEach((el)=>{
            let ts = el.getComponent(UITransform);
            totalHeight += ts.height;
        });

        
        const transform = this.content.getComponent(UITransform);
        if (transform) {
            transform.height = totalHeight;
        }
    }
    


}
