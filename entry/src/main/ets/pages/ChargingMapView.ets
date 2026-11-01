import { router } from "@kit.ArkUI";

interface Position {
  top: string;
  left: string;
}

interface RoadCondition {
  name: string;
  status: string;
  color: string;
  position: Position;
}

@Entry
@Component
struct ChargingMapView {
  private roadConditions: RoadCondition[] = [
    {
      name: "朝阳大街",
      status: "空闲",
      color: "#4CAF50",
      position: { top: "10%", left: "20%" }
    },
    {
      name: "长安中路",
      status: "满员",
      color: "#F44336",
      position: { top: "40%", left: "40%" }
    }
  ];

  build() {
    Stack() {
      Row() {
        Button('返回')
          .onClick(() => router.back())
          .margin({ left: 16 })
          .backgroundColor('#3498db')
          .width(60)
          .height(30)
          .borderRadius(15)
        Text('充电地图')
          .fontSize(20)
          .fontWeight(FontWeight.Bold)
          .fontColor(0xFFFFFF)
      }
      .width('100%')
      .height(90)
      .backgroundColor('#2c3e50')
      .alignItems(VerticalAlign.Top)
      .position({ top: 0, left: 0 })

      Image($r('app.media.map'))
        .width('100%')
        .height('calc(100% - 70px)')
        .objectFit(ImageFit.Contain)

      ForEach(this.roadConditions, (item: RoadCondition) => {
        Text(`${item.name}\n${item.status}`)
          .fontSize(14)
          .fontColor(0xFFFFFF)
          .backgroundColor(item.color)
          .padding(8)
          .borderRadius(8)
          .shadow({ color: 0x000000, offsetX: 2, offsetY: 2, radius: 2 })
          .position({
            top: item.position.top as Dimension,
            left: item.position.left as Dimension
          })
      })
    }
    .width('100%')
    .height('100%')
  }
}