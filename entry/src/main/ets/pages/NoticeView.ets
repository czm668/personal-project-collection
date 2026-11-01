@Entry
@Component
struct NoticeView {
  private noticeList: Array<NoticeItem> = [
    {
      id: 1,
      type: '活动通知',
      title: '关于开展交通安全宣传活动的通知',
      status: '新通知',
      date: '2023-05-10',
      summary: '为提高市民交通安全意识，本周六将在市民广场开展交通安全宣传活动，欢迎大家参与',
      department: '市交警支队',
      views: 128,
      hasAttachment: true
    },
    {
      id: 2,
      type: '交通管制',
      title: '某路段施工交通管制公告',
      status: '进行中',
      date: '2023-05-09',
      summary: '因道路施工，5月10日至6月30日对朝阳路东段实施交通管制，过往车辆请绕行',
      department: '市住建局',
      views: 215,
      hasAttachment: false
    },
    {
      id: 3,
      type: '业务通知',
      title: '驾驶证期满换证业务优化通知',
      status: '已生效',
      date: '2023-05-08',
      summary: '即日起驾驶证期满换证业务支持线上预约，线下办理时间缩短至30分钟以内',
      department: '车管所',
      views: 89,
      hasAttachment: true
    }
  ]

  private formatDate(dateStr: string): string {
    const date = new Date(dateStr);
    return `${date.getFullYear()}年${date.getMonth() + 1}月${date.getDate()}日`;
  }

  build() {
    Column() {

      Row() {
        Text('通知公告')
          .fontSize(24)
          .fontWeight(FontWeight.Medium)

        Text('全部')
          .fontSize(14)
          .margin({ left: 8 })
          .padding({ top: 2, bottom: 2, left: 12, right: 12 })
          .backgroundColor('#F5F5F5')
          .borderRadius(18)
      }
      .margin({ top: 20, bottom: 16 })

      List() {
        ForEach(this.noticeList, (notice: NoticeItem) => {
          ListItem() {
            Column() {
              Row() {
                Text(notice.type)
                  .fontSize(12)
                  .padding({ top: 4, bottom: 4, left: 8, right: 8 })
                  .backgroundColor(this.getTypeColor(notice.type))
                  .borderRadius(18)
                  .margin({ right: 8 })

                if (notice.status === '新通知') {
                  Text('•')
                    .fontSize(10)
                    .fontColor('#FF4D4F')
                    .margin({ top: 2 })
                }
                Text(notice.status)
                  .fontSize(12)
                  .fontColor('#666')
              }
              .margin({ bottom: 8 })

              Text(notice.title)
                .fontSize(16)
                .fontWeight(FontWeight.Medium)
                .lineHeight(22)
                .margin({ bottom: 12 })

              Text(notice.summary)
                .fontSize(14)
                .fontColor('#999')
                .lineHeight(20)
                .margin({ bottom: 16 })

              Row() {
                Text(`发布：${notice.department}`)
                  .fontSize(12)
                  .fontColor('#666')
                  .margin({ right: 24 })

                Row() {
                  Text(this.formatDate(notice.date))
                    .fontSize(12)
                    .fontColor('#666')

                  Text(` 阅读 ${notice.views}次`)
                    .fontSize(12)
                    .fontColor('#666')
                    .margin({ left: 8 })
                }

                if (notice.hasAttachment) {
                  Image($r('app.media.ic_attachment'))
                    .width(16)
                    .height(16)
                    .margin({ left: 8 })
                }
              }
              .width('100%')
              .justifyContent(FlexAlign.SpaceBetween)
            }
            .padding(16)
            .borderWidth({ bottom: 1 })
            .borderColor({ bottom: '#F0F0F0' })
          }
        }, (notice: NoticeItem) => notice.id.toString())
      }
      .margin({ left: 16, right: 16 })
      .width('100%')
    }
    .width('100%')
    .height('100%')
    .padding(20)
    .backgroundColor('#FFFFFF')
  }

  private getTypeColor(type: string): string {
    switch(type) {
      case '活动通知': return '#409EFF';
      case '交通管制': return '#FF6B6B';
      case '业务通知': return '#00BFA5';
      default: return '#909399';
    }
  }
}


interface NoticeItem {
  id: number;
  type: string;
  title: string;
  status: string;
  date: string;
  summary: string;
  department: string;
  views: number;
  hasAttachment: boolean
}