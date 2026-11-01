import router from '@ohos.router';
import prompt from '@ohos.prompt';

@Entry
@Component
export struct FinePaymentView {
  @State paymentItems: PaymentItem[] = [
    {
      id: 1,
      date: '2023-05-01',
      violation: '闯红灯',
      location: '广州市天河区天河路',
      ticketNumber: '440106123456789',
      amount: 200,
      status: 'pending'
    },
    {
      id: 2,
      date: '2023-04-15',
      violation: '不按规定车道行驶',
      location: '广州市越秀区中山路',
      ticketNumber: '440104123456789',
      amount: 100,
      status: 'pending'
    }
  ];

  @State paidItems: PaymentItem[] = [
    {
      id: 3,
      date: '2023-03-10',
      violation: '超速行驶',
      location: '广州市白云区机场路',
      ticketNumber: '440111123456789',
      amount: 150,
      status: 'paid',
      paymentDate: '2023-03-15'
    }
  ];

  build() {
    Column() {
      // 返回按钮和标题
      Row() {
        Button('返回')
          .width(80)
          .height(40)
          .backgroundColor('#FFFFFF')
          .fontColor('#007DFF')
          .borderRadius(20)
          .onClick(() => {
            router.back();
          })
          .margin({ left: 15 })

        Text('罚款缴纳')
          .fontSize(20)
          .fontWeight(FontWeight.Bold)
          .margin({ left: 100 })
      }
      .width('100%')
      .height(60)
      .backgroundColor('#FFFFFF')
      .margin({ top: 20 })

      // 内容区域
      Column() {
        // 待缴费项目
        Column() {
          Text('待缴费项目')
            .fontSize(18)
            .fontWeight(FontWeight.Bold)
            .margin({ left: 20, top: 20 })

          ForEach(this.paymentItems, (item: PaymentItem) => {
            if (item.status === 'pending') {
              PaymentItemCard({
                item: item,
                onPay: () => this.handlePayment(item.id)
              })
            }
          })
        }

        // 已缴费历史
        Column() {
          Text('已缴费历史')
            .fontSize(18)
            .fontWeight(FontWeight.Bold)
            .margin({ left: 20, top: 20 })

          ForEach(this.paidItems, (item: PaymentItem) => {
            if (item.status === 'paid') {
              PaidItemCard({ item: item })
            }
          })
        }
      }
      .width('100%')
      .backgroundColor('#F5F5F5')
    }
    .width('100%')
    .height('100%')
    .backgroundColor('#F5F5F5')
  }

  handlePayment(id: number): void {
    const itemIndex = this.paymentItems.findIndex(item => item.id === id);
    if (itemIndex!== -1) {
      const paidItem: PaymentItem = {
        id: this.paymentItems[itemIndex].id,
        date: this.paymentItems[itemIndex].date,
        violation: this.paymentItems[itemIndex].violation,
        location: this.paymentItems[itemIndex].location,
        ticketNumber: this.paymentItems[itemIndex].ticketNumber,
        amount: this.paymentItems[itemIndex].amount,
        status: 'paid',
        paymentDate: getCurrentDate()
      };

      // 已缴费数组
      const newPaidItems: PaymentItem[] = [];
      newPaidItems.push(paidItem);
      for (const item of this.paidItems) {
        newPaidItems.push(item);
      }
      this.paidItems = newPaidItems;

      // 移除待缴费项目
      this.paymentItems.splice(itemIndex, 1);

      // 显示缴费成功提示
      prompt.showToast({ message: '缴费成功！', duration: 2000 });
    }
  }
}

// 缴费项目卡片
@Component
struct PaymentItemCard {
  @Prop item: PaymentItem;
  onPay: () => void = () => {};

  build() {
    Column() {
      Row() {
        Text(`${this.item.date} ${this.item.violation}`)
          .fontSize(16)
          .margin({ left: 20, top: 10 })
      }

      Row() {
        Text(`地点: ${this.item.location}`)
          .fontSize(16)
          .margin({ left: 20, top: 10 })
      }

      Row() {
        Text(`处罚决定书编号: ${this.item.ticketNumber}`)
          .fontSize(16)
          .margin({ left: 20, top: 10 })
      }

      Row() {
        Text('罚款金额:')
          .fontSize(16)
          .margin({ left: 20, top: 10 })

        Text(`${this.item.amount}元`)
          .fontSize(18)
          .fontColor('#FF0000')
          .fontWeight(FontWeight.Bold)
          .margin({ left: 10, top: 10 })
      }

      Button('立即缴纳')
        .width(120)
        .height(40)
        .backgroundColor('#007DFF')
        .fontColor('#FFFFFF')
        .fontSize(16)
        .borderRadius(20)
        .margin({ left: 230, top: 10, bottom: 10 })
        .onClick(() => {
          this.onPay();
        })
    }
    .width('90%')
    .height(200)
    .backgroundColor('#FFFFFF')
    .borderRadius(10)
    .margin({ top: 10, left: 20 })
  }
}

// 已缴费项目卡片
@Component
struct PaidItemCard {
  @Prop item: PaymentItem;

  build() {
    Column() {
      Row() {
        Text(`${this.item.date} ${this.item.violation}`)
          .fontSize(16)
          .margin({ left: 20, top: 10 })
      }

      Row() {
        Text(`地点: ${this.item.location}`)
          .fontSize(16)
          .margin({ left: 20, top: 10 })
      }

      Row() {
        Text(`处罚决定书编号: ${this.item.ticketNumber}`)
          .fontSize(16)
          .margin({ left: 20, top: 10 })
      }

      Row() {
        Text('罚款金额:')
          .fontSize(16)
          .margin({ left: 20, top: 10 })

        Text(`${this.item.amount}元`)
          .fontSize(18)
          .fontColor('#999999')
          .margin({ left: 10, top: 10 })
      }

      Row() {
        Text(`缴费时间: ${this.item.paymentDate}`)
          .fontSize(16)
          .fontColor('#999999')
          .margin({ left: 20, top: 10 })
      }
    }
    .width('90%')
    .height(180)
    .backgroundColor('#FFFFFF')
    .borderRadius(10)
    .margin({ top: 10, left: 20, bottom: 20 })
  }
}

interface PaymentItem {
  id: number;
  date: string;
  violation: string;
  location: string;
  ticketNumber: string;
  amount: number;
  status: 'pending' | 'paid';
  paymentDate?: string;
}

// 获取当前日期
function getCurrentDate(): string {
  const date = new Date();
  const year = date.getFullYear();
  const month = String(date.getMonth() + 1).padStart(2, '0');
  const day = String(date.getDate()).padStart(2, '0');
  return `${year}-${month}-${day}`;
}

@Preview
struct FinePaymentViewPreview {
  build() {
    FinePaymentView()
  }
}