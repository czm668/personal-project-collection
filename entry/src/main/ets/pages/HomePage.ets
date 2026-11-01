import { GoodsView } from '../views/GoodsView'
import { HomeView } from '../views/HomeView'
import { ServiceView } from '../views/ServiceView'
import { UserView } from '../views/UserView'

@Entry
@Component
export struct HomePage {
  @State currentIndex:number = 0
  tabsController:TabsController = new TabsController()
  @Builder
  TabBuilder(title:string,
    targetIndex:number,
    selectedImg:Resource,
    normalImg:Resource,
  ){
    Column(){
      Image(this.currentIndex == targetIndex ? selectedImg:normalImg)
        .width(28)
        .height(28)

      Text(title)
        .fontColor(this.currentIndex == targetIndex ? '#f00':'#333')
        .padding({top:2})
    }.width('100%')
    .height(50)
    .justifyContent(FlexAlign.Center)
    .onClick(()=>{
      this.currentIndex = targetIndex
      this.tabsController.changeIndex(targetIndex)
    })
  }


  @Builder tabBuilder(title: string, targetIndex: number, selectedImg: Resource, normalImg: Resource) {
    Column() {
      Image(this.currentIndex === targetIndex ? selectedImg : normalImg)
        .size({ width: 25, height: 25 })
      Text(title)
        .fontColor(this.currentIndex === targetIndex ? '#1698CE' : '#6B6B6B')
    }
    .width('100%')
    .height(50)
    .justifyContent(FlexAlign.Center)
  }

  build() {

    Tabs({barPosition: BarPosition.End,
      index:this.currentIndex,
      controller:this.tabsController
    }){
      TabContent(){
        HomeView()
      }.tabBar(this.TabBuilder('首页',0,$r('app.media.home'),$r('app.media.home_selected')))

      TabContent(){
        ServiceView()
      }.tabBar(this.TabBuilder('服务',1,$r('app.media.fuwu'),$r('app.media.fuwu_selected')))

      TabContent(){
        GoodsView()
      }.tabBar(this.TabBuilder('消息',2,$r('app.media.zixun'),$r('app.media.zixun_selected')))

      TabContent(){
        UserView()
      }.tabBar(this.TabBuilder('我的',3,$r('app.media.wode'),$r('app.media.wode_selected')))
    }
    .scrollable(false)

    .height('100%')
    .width('100%')
  }
}