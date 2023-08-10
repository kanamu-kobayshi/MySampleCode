import os
from sqlalchemy import create_engine, Column, Integer, String, Float, Sequence
from sqlalchemy.orm import declarative_base, sessionmaker

# 現在のディレクトリ内のサブディレクトリにデータベースを作成する設定
DB_PATH = 'db/example.db'
if not os.path.exists('db'):
    os.makedirs('db')

# データベースとの接続エンジンを作成
engine = create_engine(f'sqlite:///{DB_PATH}', echo=True)

# declarative_baseを使用してBaseクラスを定義
Base = declarative_base()

# Userテーブルの定義


class User(Base):
    __tablename__ = 'users'
    id = Column(Integer, Sequence('user_id_seq'), primary_key=True)
    name = Column(String(50))
    height = Column(Float)
    weight = Column(Float)


# セッションを作成
Session = sessionmaker(bind=engine)
session = Session()

# データベースの内容を表示する関数


def display_database_content():
    # users テーブルから全てのデータを取得
    all_users = session.query(User).all()

    # データの表示
    print("ID\tName\tHeight\tWeight")
    print("-" * 40)
    for user in all_users:
        print(f"{user.id}\t{user.name}\t{user.height}\t{user.weight}")


# データベースの内容を表示
display_database_content()
