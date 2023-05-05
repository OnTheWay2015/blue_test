#coding:utf-8
import jieba
from pyltp import Segmentor
text='奎金斯距离祭台很近，加拉塔“掉落”的部分事物就在他的面前，他下意识就捡起了其中一块金属碎屑般的事物和小片黑色固体。'
segs1=jieba.cut(text)
print('|'.join(segs1))
segs1=jieba.cut_for_search(text)
print('|'.join(segs1))
segmentor=Segmentor()  #实例化分词模块
segmentor.load("D:\\ltp_data\\cws.model")
segs2=segmentor.segment(text)
print('|'.join(segs2))
segmentor.release()    #释放模型