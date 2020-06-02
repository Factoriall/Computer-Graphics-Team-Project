# Computer-Graphics-Team-Project
## 제목
- Jump Back To The Space
![](./mdImage/title.png)
## 게임 목표
- 맵 내에서 공을 튀겨서 목표 지점까지 올라가는 것이 목표
## 맵 구조 및 구조물 설명
- 전체 구조
  * 우주 및 지옥 컨셉으로 잡음  
![map](./mdImage/game_map.png)
- 발판 설명
  - Sticky Plate: 끈적끈적한 발판, 공이 착 달라붙는다.
  <img src="/mdImage/sticky_plate.png" alt="sticky" title="sticky plate" width="200" height="70"/>
  
  - Iron Plate: 딴딴한 발판, 공이 어느정도 튄다.
  <img src="/mdImage/iron_plate.png" alt="iron" title="iron plate" width="200" height="70"/>
  
  - Whitehole Plate: 점프 바닥, 공이 높게 튄다.
  <img src="/mdImage/whitehole_plate.png" alt="whitehole" title="whitehole plate" width="200" height="70"/>
  
  - Ice Plate: 미끌미끌한 바닥, 공이 살짝 미끄러진다.
  <img src="/mdImage/ice_plate.png" alt="ice" title="ice plate" width="200" height="70"/>

- 골인 지점
  - 시공의 폭풍 속으로~  
  ![storm](./mdImage/storm.png)
  
## 구현 내용
- 공 및 발판, 벽 등 fundamental structure 구축
- 인트로 & 아웃트로 & 엔딩크레딧
- 물리 엔진(physics.h)
  * 공과 벽과의 충돌
  * 중력
  * 마찰력
- 소리 및 글자 렌더링
- 3D 스카이박스, 텍스쳐
- 2D 애니메이션, 역동적인 카메라워크
- 파티클 구현
  
