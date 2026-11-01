import { router } from "@kit.ArkUI";

interface NoticeItem {
  id: number;
  title: string;
  date: string;
}

@Entry
@Component
export struct NoticeListView {
  private notices: NoticeItem[] = [
    { id: 1, title: '关于开展交通安全宣传活动的通知', date: '2023-05-10' },
    { id: 2, title: '某路段施工交通管制公告', date: '2023-05-09' },
    { id: 3, title: '驾驶证期满换证业务优化通知', date: '2023-05-08' }
  ];

  build() {
    Column() {
      Row() {
        Button('返回')
          .onClick(() => router.back())
          .margin({ left: 15 });
        Text('通知列表')
          .fontSize(20)
          .fontWeight(FontWeight.Bold);
      }
      .width('100%')
      .height(60)
      .backgroundColor('#FFFFFF')
      .justifyContent(FlexAlign.SpaceBetween);

      List({ space: 10 }) {
        ForEach(this.notices, (item: NoticeItem) => {
          ListItem() {
            Row() {
              Text(item.title)
                .fontSize(16)
                .width('80%');
              Text(item.date)
                .fontSize(14)
                .fontColor('#999999')
                .margin({ left: 30 });
            }
            .onClick(() => {
              router.pushUrl({
                url: 'pages/NoticeDetailView',
                params: { id: item.id }
              });
            });
          }
        });
      }
      .width('100%')
      .margin({ top: 20 });
    }
    .width('100%')
    .height('100%')
    .backgroundColor('#F5F5F5');
  }
}