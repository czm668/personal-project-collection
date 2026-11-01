import { router } from "@kit.ArkUI";

interface BusinessItem {
  title: string;
  icon: Resource;
  subtitle: string;
  route: string;
}

@Entry
@Component
export struct VehicleBusinessView {
  private businessItems: BusinessItem[] = [
    {
      title: '注册登记',
      icon: $r('app.media.ic_registration'),
      subtitle: '新车/二手车注册登记业务',
      route: 'pages/RegistrationPage'
    },
    {
      title: '转移登记',
      icon: $r('app.media.ic_transfer'),
      subtitle: '车辆所有权转移登记',
      route: 'pages/TransferPage'
    },
    {
      title: '变更登记',
      icon: $r('app.media.ic_modification'),
      subtitle: '车辆信息变更登记',
      route: 'pages/ModificationPage'
    }
  ];

  build() {
    Column() {
      Row() {
        Button() {
          Image($r('app.media.back'))
            .fillColor(Color.Black)
        }
        .onClick(() => router.back())
        .margin({ left: 12 })
        .width(32)
        .height(32)
        .backgroundColor(Color.Transparent);

        Text('机动车业务')
          .fontSize(24)
          .fontWeight(FontWeight.Bold)
          .margin({ left: 16 });

        Blank();

        Button() {
          Image($r('app.media.vehicle_icon'))
        }
        .width(32)
        .height(32)
        .margin({ right: 12 })
        .backgroundColor(Color.Transparent);
      }
      .width('100%')
      .height(56)
      .backgroundColor(Color.White)
      .shadow({ radius: 8, color: '#1A000000', offsetX: 0, offsetY: 4 });

      List({ space: 16 }) {
        ForEach(this.businessItems, (item: BusinessItem) => {
          ListItem() {
            Column() {
              Row() {
                Image(item.icon)
                  .width(40)
                  .height(40)
                  .margin({ right: 16 });

                Column() {
                  Text(item.title)
                    .fontSize(18)
                    .fontColor('#333333')
                    .fontWeight(FontWeight.Medium);

                  Text(item.subtitle)
                    .fontSize(14)
                    .fontColor('#666666')
                    .margin({ top: 4 });
                }
                .alignItems(HorizontalAlign.Start)

                Blank();

                Image($r('app.media.ic_arrow_right'))
                  .width(24)
                  .height(24)
              }
              .padding(16)
            }
            .backgroundColor(Color.White)
            .borderRadius(12)
            .width('93%')
            .shadow({ radius: 4, color: '#10000000', offsetX: 0, offsetY: 2 })
          }
          .onClick(() => {
            router.pushUrl({ url: item.route });
          })
          .stateStyles({
            pressed: {
              opacity: 0.6
            }
          })
        }, (item: BusinessItem) => item.route)
      }
      .width('100%')
      .margin({ top: 24 })
      .scrollBar(BarState.Off)
    }
    .width('100%')
    .height('100%')
    .backgroundColor('#F9F9F9')
  }
}