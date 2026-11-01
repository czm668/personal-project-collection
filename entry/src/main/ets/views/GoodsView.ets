import { router } from "@kit.ArkUI";

@Observed
class NewsItem {
  name: string;
  image: ResourceStr;
  date: number;
  category: string;

  constructor(name: string, image: ResourceStr, date: number, category: string = "全部") {
    this.name = name;
    this.image = image;
    this.date = date;
    this.category = category;
  }
}

@Component
export struct GoodsView {
  private items: NewsItem[] = [
    new NewsItem('民警向茶农宣讲交通安全知识', $r('app.media.tea'), 20250425, "安全"),
    new NewsItem('车辆高架爆胎 铁骑及时解困', $r('app.media.tai'), 20250425, "交通"),
    new NewsItem('福建省安全形势稳定向好', $r('app.media.anquan'), 20250425, "安全"),
    new NewsItem('宣传交通安全 筑牢安全防线', $r('app.media.gongjiao'), 20250425, "交通"),
    new NewsItem('龙岩公安全面开启"护学模式"', $r('app.media.huxue'), 20250425, "通知"),
    new NewsItem('左毅30载坚守，不下班的警察', $r('app.media.zuoyi'), 20250425, "安全"),
    new NewsItem('暴雨天气交警全员上路疏导交通', $r('app.media.baodao'), 20250426, "交通"),
    new NewsItem('校园安全演练提升师生应急能力', $r('app.media.baodao'), 20250426, "安全"),
    new NewsItem('关于五一假期交通管制的最新通知', $r('app.media.baodao'), 20250426, "通知"),
    new NewsItem('智能安防系统助力社区安全升级', $r('app.media.baodao'), 20250426, "安全"),
    new NewsItem('新能源汽车充电安全知识科普活动', $r('app.media.baodao'), 20250426, "安全"),
    new NewsItem('早晚高峰时段新增公交专用车道通告', $r('app.media.baodao'), 20250426, "交通"),
  ];

  @State selectedCategory: string = "全部";
  private categories: string[] = ["全部", "交通", "安全", "通知"];

  build() {
    Stack({ alignContent: Alignment.Top }) {
      Column() {
        Row() {
          Text('消息')
            .fontSize(28)
            .fontColor('#000000')
            .fontWeight(700);
        }
        .width('100%')
        .height(60)
        .justifyContent(FlexAlign.Start)
        .padding({ left: 20 })
        .backgroundColor('#F5F5F5');

        Row({ space: 10 }) {
          Image($r('app.media.search_icon'))
            .onClick(() => {
              router.pushUrl({
                url: 'pages/SearchView'
              });
            })
            .width(24)
            .height(24)
            .margin({ left: 15 });

          Text('搜索资讯...')
            .fontSize(16)
            .fontColor('#999999')
            .opacity(0.8);
        }
        .width('90%')
        .height(48)
        .backgroundColor(Color.White)
        .borderRadius(24)
        .shadow({ radius: 4, color: '#10000000', offsetX: 2, offsetY: 2 })
        .margin({ bottom: 16 })
        .onClick(() => {
          router.pushUrl({ url: 'views/SearchView' });
        });

        Scroll() {
          Row({ space: 12 }) {
            ForEach(this.categories, (category: string) => {
              Text(category)
                .fontSize(16)
                .fontWeight(this.selectedCategory === category? 700 : 500)
                .padding({ left: 20, right: 20, top: 10, bottom: 10 })
                .backgroundColor(this.selectedCategory === category? '#007DFF' : '#F5F5F5')
                .fontColor(this.selectedCategory === category? Color.White : '#333333')
                .borderRadius(20)
                .onClick(() => {
                  this.selectedCategory = category;
                });
            });
          }
          .padding({ right: 24 });
        }
        .scrollable(ScrollDirection.Horizontal)
        .scrollBar(BarState.Off)
        .width('100%')
        .height(56);
      }
      .width('100%')
      .zIndex(2)
      .backgroundColor('#F5F5F5');

      Scroll() {
        List({ space: 32 }) {
          ForEach(this.items.filter(item =>
          this.selectedCategory === "全部" || item.category === this.selectedCategory
          ), (item: NewsItem) => {
            ListItem() {
              Row({ space: 16 }) {
                Image(item.image)
                  .width(120)
                  .height(120)
                  .aspectRatio(1)
                  .objectFit(ImageFit.Cover)
                  .borderRadius(12);

                Column({ space: 8 }) {
                  Text(item.name)
                    .fontSize(18)
                    .fontWeight(FontWeight.Medium)
                    .maxLines(2)
                    .textOverflow({ overflow: TextOverflow.Ellipsis })
                    .lineHeight(24);

                  Row() {
                    Text(this.formatDate(item.date))
                      .fontSize(14)
                      .fontColor('#666666');

                    Blank();

                    Text(item.category)
                      .fontSize(14)
                      .fontColor('#007DFF')
                      .backgroundColor('#E8F3FF')
                      .padding({ left: 12, right: 12, top: 4, bottom: 4 })
                      .borderRadius(16);
                  }
                  .width('100%');
                }
                .layoutWeight(1);
              }
              .width('94%')
              .padding(16)
              .backgroundColor(Color.White)
              .borderRadius(24)
              .shadow({ radius: 8, color: '#10000000', offsetX: 2, offsetY: 4 })
              .onClick(() => {
                router.pushUrl({
                  url: 'pages/NewsDetailView',
                  params: {
                    title: item.name,
                    image: item.image,
                    date: this.formatDate(item.date),
                    category: item.category
                  }
                });
              });
            }
          });
        }
        .width('100%')
        .margin({ top: 160 })
        .edgeEffect(EdgeEffect.None);
      }
      .width('100%')
      .height('100%')
      .margin({ top: 200 });
    }
    .height('100%')
    .backgroundColor('#F5F5F5');
  }

  private formatDate(timestamp: number): string {
    const dateStr = timestamp.toString();
    if (dateStr.length!== 8) return '2024-01-01';
    return `${dateStr.substring(0, 4)}-${dateStr.substring(4, 6)}-${dateStr.substring(6, 8)}`;
  }
}