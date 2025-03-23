# Frost

## 프로젝트 개요
- **서리가 성장하는 절차적 애니메이션 구현**
- **Mesh의 표면을 따라 성장하도록 구현**

<br>

<div align="center">
  <table>
    <tr>
      <th> 개발 환경 </th>
      <th> 개발 인원 </th>
      <th> 개발 기간 </th>
    </tr>  
    <tr>
      <td align="center"> <code> DirectX 11 </code> <code> C++ </code> </td>
      <td align="center"> <code> 본인 1명 </code> </td>
      <td align="center"> <code> 2025.01.16 ~ 2025.03.18 </code> </td>
    </tr>
  </table>
</div>

<br>

## 시연 영상
https://github.com/user-attachments/assets/0bd93aac-38e4-4b91-a257-6e642917485d  

<br>

## 서리 성장 알고리즘
<div align="center">
  <table>
    <tr>
      <th align="center" width="360px"> 1. Mesh의 표면에 서리의 핵을 <br> 여러개 배치한다. </span> </th>
      <th align="center" width="360px"> 2. 무작위 서리의 핵에서 <br> 서리의 성장을 시작한다. </th>
      <th align="center" width="360px"> 3. 서리는 Mesh의 표면을 따라 <br> 성장한다. </th>
    </tr>  
    <tr>
      <td align="center"> <img src="https://github.com/user-attachments/assets/9a8f0ece-0d7a-4b42-a5c4-a40889f82399" width="300px" height="150px"/> </td>
      <td align="center"> <img src="https://github.com/user-attachments/assets/9668ae66-127e-4b65-a609-58f2d65aa98d" width="300px" height="150px"/> </td>
      <td align="center"> <img src="https://github.com/user-attachments/assets/8dc54720-09a7-4863-8c1e-e22d45e77c1f" width="300px" height="150px"/> </td>
    </tr>
    <tr>
      <th align="center"> 4. 성장하는 서리는 가지에서 <br> Sub 가지가 뻗어나간다. </td>
      <th align="center"> 5. 서리가 성장하지 않은 핵과 가까워지면 그 핵에서 서리의 성장을 시작한다. </td>
      <th align="center"> 6. 서리가 다른 서리와 가까워지면 <br> 성장을 멈춘다. </td>
    </tr>
    <tr>
      <td align="center"> <img src="https://github.com/user-attachments/assets/8ed2a445-8ccd-4551-8910-c4c81f063992" width="300px" height="150px"/> </td>
      <td align="center"> <img src="https://github.com/user-attachments/assets/4893d1a9-1bfc-4983-8413-c8409e2ddadc" width="300px" height="150px"/> </td>
      <td align="center"> <img src="https://github.com/user-attachments/assets/ecc826f2-6282-4acb-876c-17201921158e" width="300px" height="150px"/> </td>
    </tr>
  </table>
</div>  
  
<br>

## 서리가 표면을 따라 성장하는 방법

<div align="center">
  <table>
    <tr>
      <th> 설명 </th>
      <th> 이미지 </th>
    </tr>  
    <tr>
      <td align="center">  </td>
      <td align="center"> </td>
    </tr>
  </table>
</div>

## 성장 지점의 수학적 배경
Mesh의 표면을 따라 성장하기 위해 원의 자취와 삼각형이 만나는 교차점을 찾습니다.  
교차점을 찾는 과정은 직선과 삼각형이 만나는 Raycast 과정에서 착안하여 구현했습니다.

- 일반적인 Raycast 과정
    - 평면의 방정식은 다음과 같이 나타낼 수 있다.  
      `Ax + By + Cz = D`  
    - 기하학적인 관점에서 정리하면 다음과 같다.  
      `Plane.Normal * P(x, y, z) = Plane.Offset`  
    - 교차점 P를 t에 대한 매개변수로 나타내면 다음과 같다.  
      `P(t) = Ray.origin + t * Ray.direction`  
    - 따라서 다음을 만족하는 `t`값을 구한다.  
      `Plane.Normal * P(t) = Plane.Offset`
    - P(t)가 삼각형 내부에 존재하는지 판별한다.   

- 원의 자취와 삼각형의 교차점 찾는 과정
    - 평면의 방정식은 다음과 같이 나타낼 수 있다.  
      `Ax + By + Cz = D`  
    - 기하학적인 관점에서 정리하면 다음과 같다.  
      `Plane.Normal * P(x, y, z) = Plane.Offset`
    - 교차점 P를 theta에 대한 매개변수로 나타내면 다음과 같다.  
      `P(theta) = Circle.center + Circle.radius * (cos(theta) * Circle.axisX + sin(theta) * Circle.axisY)`
    - 따라서 다음을 만족하는 `theta` 값을 구한다. (삼각함수의 합성을 이용한다)  
      `Plane.Normal * P(theta) = Plane.Offset`  
      `A'cos(theta) + B'sin(theta) = C'`
    - P(theta)가 삼각형 내부에 존재하는지 판별한다.
      
## Octree를 이용한 탐색 최적화
서리 성장 과정 중 다음 두 가지에는 가까운 서리를 탐색하는 로직이 필요합니다.  
- `성장한 서리가 성장을 시작하지 않은 핵과 가까워지면 그 핵에서 서리의 성장이 시작된다.`  
- `서리가 다른 서리와 가까워지면 성장을 멈춘다.`

모든 서리를 탐색한다면 서리가 성장할수록 탐색해야 할 대상이 선형으로 증가합니다. ( O(n) )  
따라서 서리의 위치는 Octree를 이용하여 저장하고, 특정 Bound 이내의 서리만 탐색하도록 합니다. ( O(log n) )
