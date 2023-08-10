from sqlalchemy import create_engine, Column, Integer, String, Float, Sequence
from sqlalchemy.orm import declarative_base, sessionmaker

# データベースとの接続エンジンを作成
engine = create_engine('sqlite:///db/example.db')


# declarative_baseを使用してBaseクラスを定義
Base = declarative_base()

# Userテーブルの定義


class User(Base):
    __tablename__ = 'users'
    id = Column(Integer, Sequence('user_id_seq'), primary_key=True)
    name = Column(String(50))
    height = Column(Float)
    weight = Column(Float)


# テーブルを作成
Base.metadata.create_all(engine)

# セッションを作成
Session = sessionmaker(bind=engine)
session = Session()

# 10人のデータを追加
users_data = [
    {"name": "User1", "height": 170.1, "weight": 65.2},
    {"name": "User2", "height": 165.3, "weight": 54.5},
    {"name": "User3", "height": 172.6, "weight": 74.8},
    {"name": "User4", "height": 160.0, "weight": 58.9},
    {"name": "User5", "height": 175.2, "weight": 76.5},
    {"name": "User6", "height": 168.4, "weight": 70.1},
    {"name": "User7", "height": 162.7, "weight": 62.3},
    {"name": "User8", "height": 174.9, "weight": 68.7},
    {"name": "User9", "height": 161.2, "weight": 59.6},
    {"name": "User10", "height": 173.5, "weight": 72.4},
]

for user_data in users_data:
    user = User(
        name=user_data["name"], height=user_data["height"], weight=user_data["weight"])
    session.add(user)

session.commit()
