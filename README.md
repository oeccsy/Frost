# Frost

## 프로젝트 개요
> ### 메쉬의 표면을 따라 자라나는 서리 애니메이션

<div align="center">
  <img src="https://github.com/user-attachments/assets/55468110-6fb0-4fd4-8729-4238c6a39efa" width="70%">
</div>

<div align="center">
  <table>
    <tr>
      <th> 프로젝트 유형 </th>
      <th> 개발 인원 </th>
      <th> 개발 기간 </th>
    </tr>  
    <tr>
      <td align="center"> <code> 절차적 애니메이션 </code> <code> 테크 데모 </code> </td>
      <td align="center"> <code> 본인 1명 </code> </td>
      <td align="center"> <code> 2025.01.16 ~ 2025.03.18 </code> </td>
    </tr>
  </table>
</div>

<div align="center">
  <table>
    <tr>
      <th> 개발 환경 </th>
      <th> 활용 기술 </th>
    </tr>  
    <tr>
      <td align="center"> <code> DirectX 11 </code> <code> C++ </code> </td>
      <td align="center"> <code> 3D Math </code> <code> Raycast </code> <code> Octree </code> <code> BVH </code> </td>
    </tr>
  </table>
</div>

<br>

## 개발 목표
- **서리가 성장하는 절차적 애니메이션 구현**
- **Mesh의 표면을 따라 성장하여 어떤 모델에서도 수행 가능하도록 구현**

<br>

## 서리 성장 알고리즘
<div align="center">
  <table>
    <tr>
      <td align="center" width="360px"> 1. Mesh 표면에 서리 핵을 여러개 배치한다. </td>
      <td align="center" width="360px"> 2. 임의의 서리 핵에서 성장을 시작한다. </td>
      <td align="center" width="360px"> 3. 서리는 Mesh 표면을 따라 성장한다. </td>
    </tr>  
    <tr>
      <td align="center"> <img src="https://github.com/user-attachments/assets/231f45f1-4f57-45e7-a9a2-03fb0d73124a" width="300px" height="150px"/> </td>
      <td align="center"> <img src="https://github.com/user-attachments/assets/a5ba0b29-bb36-477b-b258-37e0cbfbaf11" width="300px" height="150px"/> </td>
      <td align="center"> <img src="https://github.com/user-attachments/assets/0fbcd2c2-1b6e-4134-871c-167730ca4d0d" width="300px" height="150px"/> </td>
    </tr>
    <tr>
      <td align="center"> 4. 성장하는 가지에서 Sub 가지가 생성된다. </td>
      <td align="center"> 5. 핵이 서리와 가까워지면 그 핵에서 <br> 성장을 시작한다. </td>
      <td align="center"> 6. 서리 가지가 다른 가지와 가까워지면 <br> 성장을 멈춘다. </td>
    </tr>
    <tr>
      <td align="center"> <img src="https://github.com/user-attachments/assets/b48fbca0-1583-45c9-9a6e-7ca60b7e17c9" width="300px" height="150px"/> </td>
      <td align="center"> <img src="https://github.com/user-attachments/assets/52304dd4-8607-471b-a344-31e4df72d13c" width="300px" height="150px"/> </td>
      <td align="center"> <img src="https://github.com/user-attachments/assets/a4faabf9-9200-4a66-b8bc-b2de33fbc0f1" width="300px" height="150px"/> </td>
    </tr>
  </table>
</div>  
  
<br>

## 서리가 표면을 따라 성장하는 방법
  
<img width="80%" src="https://github.com/user-attachments/assets/861239f9-3d7e-4366-877c-6f090b4fabdb" />  
  
1. 서리의 마지막 두 점으로부터 서리의 direction과 normal을 결정합니다.  
2. direction과 normal을 활용하여 3차원상 원 하나를 특정합니다.  
  
<br>
  
<img width="80%" src="https://github.com/user-attachments/assets/a8dc5c1d-c803-4c37-9c4b-66112010e1b0" />  
  
3. 원과 mesh의 polygon이 충돌하는 지점을 탐색합니다.  
4. 처음으로 충돌하는 위치를 다음 성장 지점으로 하면 표면을 따라 성장합니다.  

<br>

> [!IMPORTANT]
> 다음 성장 지점을 고르기 위해 원을 활용한 이유  
<img width="80%" src="https://github.com/user-attachments/assets/32454fee-5760-414e-967f-faf45ceeeddd" />

1. **메쉬의 다양한 굴곡 형태에 오류 없이 동작하기 위함**  
raycast를 활용해 다음 성장 지점을 선택하는 방식을 고려하기도 했으나 ray가 메쉬 표면에 충돌하도록 방향을 특정하기 어렵다는 문제가 존재
  
<br>
  
<div align="left">
  <table>
    <tr>
      <td align="center"> <img src="https://github.com/user-attachments/assets/e8f9666d-f9ed-4eb4-9e08-f2f23acb38a0" width="400px" height="300px"/> </td>
      <td align="center"> <img src="https://github.com/user-attachments/assets/65495b62-e106-4e3c-8a16-cb411d1e6f54" width="400px" height="300px"/> </td>
    </tr>  
    <tr>
      <td align="center" width="360px"> ▲ 가장 가까운 polygon에 snap 하는 경우 </td>
      <td align="center" width="360px"> ▲ 원과의 충돌 지점을 이용하는 경우 </td>
    </tr>
  </table>
</div>  

2. **서리의 성장 방향을 보존하기 위함**  
서리의 성장 방향을 보존하지 않으면 서리가 지그재그로 꺾이는 문제가 존재

<br>

## 충돌 지점의 수학적 배경
<img width="90%" src="https://github.com/user-attachments/assets/d7aabcfd-8a91-434f-a6f5-9b6510a8eae5" />

Mesh의 표면을 따라 성장하기 위해 원의 자취와 삼각형이 만나는 교차점을 찾습니다.  
교차점을 찾는 과정은 직선과 삼각형이 만나는 Raycast 과정에서 착안하여 구현했습니다.

<br>

## Octree를 이용한 탐색 최적화
<img width="70%" src="https://github.com/user-attachments/assets/5c5b1889-1e9c-4139-ac7b-60a0391710b0" />

서리 성장 과정에서는 다른 서리와 가까워졌는지 판별하는 로직이 필요했습니다.  
- `핵이 서리와 가까워지면 그 핵에서 서리의 성장을 시작한다.`  
- `서리 가지가 다른 가지와 가까워지면 성장을 멈춘다.`

모든 서리를 순차탐색 한다면 서리가 성장할수록 탐색해야 할 대상이 선형으로 증가합니다. ( O(n) )  
따라서 서리의 위치는 Octree를 이용하여 저장하고, 특정 Bound 이내의 서리만 탐색하도록 했습니다. ( O(log n) )
