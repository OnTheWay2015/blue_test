import { _decorator, Asset, assetManager, AssetManager, Component, error, ImageAsset, instantiate, Node, Prefab, resources, Sprite, SpriteFrame, Texture2D } from 'cc';
const { ccclass, property } = _decorator;

@ccclass('TestAssert')
export class TestAssert extends Component {
    async start() {
        //await this.testPrefab();
        //await this.testIMG();
        //await this.testBundle();
        //await this.testRemoteBundle();
    }

    update(deltaTime: number) {
        
    }

private async testBundle(){
    let bundleUrl= "texture";  //本地的 名为 texture 的 bundle
    let self = this;
    return new Promise((resolve, reject) => {
        assetManager.loadBundle(bundleUrl, (err, bundle: AssetManager.Bundle) => {
            if (err) {
                return reject(err);
            }
            resolve(bundle);
        });
    });


}

private async testPrefab(){
    let prefabUrl= "prefabs/Node";
    let self = this;
    let res = await self.loadResSync<Prefab>(prefabUrl, Prefab, this._onProgress);
  
    //let n:Node= instantiate(res) as Node;
    let n:Node= instantiate(res!);

    self.node.addChild(n);

}

private async testIMG(){
    let url= "imgs/home_page";
    let self = this;
    let res = await self.loadResSync<SpriteFrame>(url,SpriteFrame,this._onProgress);
    //let r:SpriteFrame= instantiate(res!);
    let r:SpriteFrame= res!;
    let sp:Sprite= self.node.addComponent(Sprite); 
    sp.spriteFrame = r;

}

/*
将 ImageAsset 添加到 Node 需要先将其转换为 Texture2D 或 SpriteFrame 等可渲染资源
*/
    private testIMG1() {
        let url = "imgs/home_page";
        let self = this;
        let res = resources.load(url, ImageAsset, self._onProgress, (err, asset: any) => {
            let info = resources.getInfoWithPath(url);
            console.log(info);
            if (err) {
                console.log(`${url} [资源加载] 错误 ${err}`);
            } else {
                console.log(`${url} [资源加载] ok`);
                let r:ImageAsset= asset as ImageAsset;
               
                let t = new Texture2D();
                t.image = r;
                const spf = new SpriteFrame();
                let sp: Sprite = self.node.addComponent(Sprite);
                spf.texture = t;
                sp.spriteFrame = spf;
            }
        });

    }


    ///** 加载资源 */
// typeof Asset，表示它需要传入一个 ‌Asset 类的构造函数‌（即类本身，而不是实例）
public loadResSync<T>(url: string, type: typeof Asset, onProgress?: (completedCount: number, totalCount: number, item: any) => void): Promise<T|null> {
    return new Promise((resolve, reject) => {
        if (!onProgress) onProgress = this._onProgress;
        resources.load(url, type, onProgress, (err, asset: any) => {
            if (err) {
                error(`${url} [资源加载] 错误 ${err}`);
                resolve(null);
            } else {
                resolve(asset as T);
            }
        });
    });
}

//    /** 
//     * 加载进度
//     * cb方法 其实目的是可以将loader方法的progress
//     */
private _onProgress(completedCount: number, totalCount: number, item: any) {
}


private async testRemoteBundle(){
    let host = "http://localhost/"
    //let host = "";
    let bundleUrl=  host + "texture/";  // localhost 上 名为 texture 的 bundle (bundle文件形式上是一个目录)
    //todo ab包更新后，要怎样更新对应的版本？
    let self = this;
    return new Promise((resolve, reject) => {
        assetManager.loadBundle(bundleUrl, (err, bundle: AssetManager.Bundle) => {
            if (err) {
                return reject(err);
            }
            let f = ()=> 
            {
                let spath = "about_down"
                let info = bundle.getInfoWithPath(spath);
                console.log(info);

                //图片资源加载使用
                bundle.load(spath, (e, data: Asset) => {
                    let r: ImageAsset = data as ImageAsset;
                    let t = new Texture2D();
                    t.image = r;
                    const spf = new SpriteFrame();
                    let sp: Sprite = self.node.addComponent(Sprite);
                    spf.texture = t;
                    sp.spriteFrame = spf;
                });

            }
            let ff = ()=> 
            {
                let spath = "xxx"
                let info = bundle.getInfoWithPath(spath);
                console.log(info);


                bundle.load(spath, (e, data:Prefab) => {
                    let r:Prefab= data as Prefab;
                    let n:Node= instantiate(r!);
                    self.node.addChild(n);
                });

            }

            f();
            ff();



            resolve(bundle);
        });
    });


}


private async loadRemoteBundle(bundleName: string) {
    // 这里的地址需要和构建配置一致，额外追加上“remote”
    // 构建那里配置的url引擎会自动追加“remote/”去请求资源
    // 两边地址不一样的话，就需要在服务器那里处理两种路由地址
    //let url = 'http://192.168.1.22:3000/assets/remote/';
    let url = 'http://localhost:80/';
    const bundleUrl = url + bundleName;
    return new Promise((resolve, reject) => {
        assetManager.loadBundle(bundleUrl, (err, bundle: AssetManager.Bundle) => {
            if (err) {
                return reject(err);
            }
            resolve(bundle);
        });
    });
}


}



